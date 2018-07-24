#pragma once

#include "Timer.h"
#include "CommonStates.h"
//#include "World.h"
#include <locale>

#pragma comment (lib, "d3d11.lib")

//폰트 새로운 시도 
//DrawWirte로 했으나 TK와의 충돌로 안됨
#include "Dwrite.h"
#include "D2D1.h"
#include "D2D1helper.h"
#pragma comment (lib, "Dwrite.lib")
#pragma comment (lib, "d2d1.lib")

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
	
	//DrawWirte로 했으나 TK와의 충돌로 안됨
	//IDWriteFactory*			pDWriteFactory_;
	//IDWriteTextFormat*		pTextFormat_;
	//ID2D1Factory*		pD2DFactory_0;
	//ID2D1HwndRenderTarget* pRT;
	//ID2D1SolidColorBrush* pBlackBrush_;
	//const wchar_t* wszText_;
	//UINT32 cTextLength_;
	//D2D1_RECT_F layoutRect;

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

