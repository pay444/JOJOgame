#pragma once

#include "Timer.h"
#include "CommonStates.h"
//#include "World.h"

#pragma comment (lib, "d3d11.lib")

//#define DEFAULT_SCREEN_WIDTH	960
//#define DEFAULT_SCREEN_HEIGHT	576

class MFramework
{
public:
	MFramework();
	virtual ~MFramework();

protected:
	Microsoft::WRL::ComPtr<IDXGISwapChain>			mspSwapchain;
	Microsoft::WRL::ComPtr<ID3D11Device>			mspDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mspDeviceCon;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mspTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mspDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mspDepthStencilView;
	std::unique_ptr<DirectX::Keyboard>				mspKeyboard;
	std::unique_ptr<DirectX::Mouse>					mspMouse;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	mspRaster;

	unique_ptr<SpriteBatch>							mspSpriteBatch;
	std::unique_ptr<SpriteFont>						mspspriteFont;

	unique_ptr<CommonStates>	mspStates;

	UINT		mScreenWidth;
	UINT		mScreenHeight;
	bool		mMinimized;
	bool		mMaximized;
	bool		mResizing;

	bool		mPaused;
	HWND		mHwnd;
	wstring		mTitleCaption;
	Timer	mTimer;

protected:
	void CalculateFPS();

public:
	static DirectX::Mouse::ButtonStateTracker				mMouseTracker;
	static DirectX::Keyboard::KeyboardStateTracker			mKeyboardTracker;
	//static std::unique_ptr<SpriteBatch> spriteBatch(new SpriteBatch(mspDeviceCon));
public:
	virtual void InitD3D(HWND hWnd);
	virtual void ClearD3D();
	virtual void Update(float dt);
	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();
	virtual void OnResize();
	
public:
	void InitWindow(HINSTANCE hInstance, LPCTSTR title = L"JoJo", UINT width = 1280, UINT height = 720);
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int GameLoop();
};

