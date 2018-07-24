#include "stdafx.h"
#include "ResourceManager.h"


ResourceManager::ResourceManager()
	:mhWnd(0),
	mpDevice(0)
{
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::Release()
{
	for (auto& resource : mShaderResources)
	{
		resource.second.Reset();	//초기화
	}
	mShaderResources.clear();
	for (auto& sheet : mSpriteSheets)
	{
		sheet.second.reset();//uniqe_ptr이라서 소문자
	}
	mSpriteSheets.clear();
}

ID3D11ShaderResourceView * ResourceManager::GetShaderResource(wstring filename)
{
	//중복이냐?
	// 중복이면? 기존에 있던거
	//신규면?
	//new 메모리 할당

	//찿고 넣고 2번이 낭비됨 찾는걸 결국 2번함 찾고 찾고 넣고
	//auto iter = this->mShaderResources.find(filename);
	//if (iter == mShaderResources.cend())
	//{
	//	//Load
	//	mShaderResources.insert();
	//}
	//else
	//	iter->second

	//이렇게하면 찾는것과 바로다음에 신규나 기존이냐 에따라서 내용을 돌려줌
	//auto map = std::map<string, string>();
	//auto result = map.insert(pair<string, string>("1", "abcd")); //일단 집어넣고 중복이냐 아니냐 확인해야함
	//if (result.second == true)
	//{
	//	//신규로 insert 된것
	//	result.first->second;//이것이  abcd iter가 first
	//}
	//else
	//	result.first->second;		//이미존재하는 iterator가 first

	//auto map = std::map<string,SGASpriteSheet*>();
	//auto result = map.insert(pair<string, string>("1", nullptr)); //일단 집어넣고 중복이냐 아니냐 확인해야함
	//if (result.second == true)
	//{
	//	//신규로 insert 된것
	//	result.first->second= new SGASpriteSheet();	// 이기법을 자주쓰게됨
	//}
	//else
	//	result.first->second;		//이미존재하는 iterator가 first

	if (mpDevice == NULL)
	{
		DXTRACE_ERR_MSGBOX(L"mpDevice is NULL. /nCall Init() first!", 0);
		return NULL;
	}
	ID3D11ShaderResourceView* pResource = NULL;

	auto result = mShaderResources.insert(
		pair<wstring, ComPtr<ID3D11ShaderResourceView>>(filename, nullptr));

	if (result.second == true)
	{
		result.first->second = ComPtr<ID3D11ShaderResourceView>();
		CreateWICTextureFromFile(mpDevice, filename.c_str(), NULL, result.first->second.GetAddressOf());
		pResource = result.first->second.Get();
		
	}
	else
		pResource = result.first->second.Get();

	return pResource;
}

SpriteSheet * ResourceManager::GetSpriteSheet(wstring filename, ID3D11ShaderResourceView * pTexture)
{
	SpriteSheet* pSheet = NULL;

	auto result = mSpriteSheets.insert(
		pair<wstring, unique_ptr<SpriteSheet>>(filename, nullptr)
	);
	if (result.second == true)
	{
		//nullptr 아니면 reset 해야함
		result.first->second = move(make_unique<SpriteSheet>());
		pSheet = result.first->second.get();
		pSheet->Load(pTexture, filename.c_str());
	}
	else
		pSheet = result.first->second.get();

	return pSheet;
}

SpriteSheet * ResourceManager::GetSpriteSheet(wstring filename, wstring textureName)
{
	return GetSpriteSheet(filename,GetShaderResource(textureName));
}
