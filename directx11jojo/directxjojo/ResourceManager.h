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
	//com�� ���ҿ����� unique_ptr�� c++ �̸��
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
	ID3D11ShaderResourceView*	GetShaderResource(wstring filename);	//�ؽ��͸� �����ͷ� ������
	SpriteSheet*				GetSpriteSheet(wstring filename,ID3D11ShaderResourceView* pTexture);
	SpriteSheet*				GetSpriteSheet(wstring filename, wstring textureName);
};

