#pragma once
class ResourceManager final
{
public:
	static ResourceManager& Instance()
	{
		static ResourceManager instance;

		return instance;
	}
private:
	ResourceManager();
	ResourceManager(ResourceManager const&);
	void operator = (ResourceManager const&);
public:
	~ResourceManager();

private:
	ID3D11Device*	mpDevice;
	HWND			mhWnd;
	//com은 마소에서만 unique_ptr은 c++ 이면다
	map<wstring, ComPtr<ID3D11ShaderResourceView>>		mShaderResources;
	map < wstring, unique_ptr<SpriteSheet>>			mSpriteSheets;

public:
	void Init(HWND hWnd, ID3D11Device* pDevice)
	{
		mhWnd = hWnd;
		mpDevice = pDevice;
	}
	void Release();
public:
	ID3D11ShaderResourceView*	GetShaderResource(wstring filename);	//텍스터를 포인터로 돌려줌
	SpriteSheet*				GetSpriteSheet(wstring filename,ID3D11ShaderResourceView* pTexture);
	SpriteSheet*				GetSpriteSheet(wstring filename, wstring textureName);
};

