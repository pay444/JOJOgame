#include "stdafx.h"
#include <sstream>
#include "SoundClass.h"
#include "Resource.h"

//폰트를 위한 헤더 안됨
//#include <d2d1helper.h>
//#include <d2d1.h>
//#pragma comment(lib, "d2d1.lib")
//#include <dwrite.h>
//#pragma comment (lib, "dwrite.lib")

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

MFramework* gpDispatch = 0;
 DirectX::Mouse::ButtonStateTracker MFramework::mMouseTracker;
 DirectX::Keyboard::KeyboardStateTracker MFramework::mKeyboardTracker;

LRESULT CALLBACK GlobalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return gpDispatch->WindowProc(hWnd, message, wParam, lParam);
}


MFramework::MFramework() : 
	mScreenWidth(DEFAULT_SCREEN_WIDTH), //STARTSCENE_SCERRN_WIDTH
	mScreenHeight(DEFAULT_SCREEN_HEIGHT), //STARTSCENE_SCERRN_HEIGHT
	mPaused(false),
	mMinimized(false),
	mMaximized(false),
	mResizing(false)
{
	gpDispatch = this;
	_wsetlocale(LC_ALL, L"korean");
}

MFramework::~MFramework()
{
}

void MFramework::InitD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = mScreenWidth;
	scd.BufferDesc.Height = mScreenHeight;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&mspSwapchain,
		&mspDevice,
		NULL,
		&mspDeviceCon);

	ResourceManager::Instance().Init(hWnd, mspDevice.Get());

	mspKeyboard = make_unique<DirectX::Keyboard>();
	mspMouse = make_unique<DirectX::Mouse>();
	mspMouse->SetWindow(hWnd);

	mspSpriteBatch = make_unique<SpriteBatch>(mspDeviceCon.Get());
	//(new SpriteFont(device, L"myfile.spritefont"));
	//mspspriteFont = make_unique<SpriteFont>(mspDevice.Get(), L"myfile.spritefont");
	mspspriteFont = make_unique<SpriteFont>(mspDevice.Get(), L"gungseo.spritefont");
	
	mspStates = std::make_unique<CommonStates>(mspDevice.Get());

	
	//ComPtr<IDWriteFactory2> writeFactory;

	//D3D11_RASTERIZER_DESC rasterDesc;
	//rasterDesc.AntialiasedLineEnable = false;
	//rasterDesc.CullMode = D3D11_CULL_BACK;
	//rasterDesc.DepthBias = 0;
	//rasterDesc.DepthBiasClamp = 0.0f;
	//rasterDesc.DepthClipEnable = true;
	//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	//rasterDesc.FrontCounterClockwise = false;
	//rasterDesc.MultisampleEnable = false;
	//rasterDesc.ScissorEnable = false;
	//rasterDesc.SlopeScaledDepthBias = 0.0f;
	//
	////작성한 description으로 부터 래스터화기 상태를 생성합니다.
	//if (FAILED(mspDevice->CreateRasterizerState(&rasterDesc, mspRaster.ReleaseAndGetAddressOf())))
	//{
	//	MessageBox(hWnd, L"rasterizer faild", L"message Box", MB_OK);
	//}

	////래스터화기 상태를 설정합니다.
	//mspDeviceCon->RSSetState(mspRaster.Get());

	//rasterDesc.FillMode = D3D11_FILL_SOLID;
	//if (FAILED(mspDevice->CreateRasterizerState(&rasterDesc, &mRasterSolidState)))
	//{
	//	MessageBox(hWnd, L"rasterizer faild", L"message Box", MB_OK);
	//}

	//Drawing Write 했으나 TK와 의 충돌로 안됨
	
 	//HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_0);
 	////if (SUCCEEDED(hr))
 	//{
 	//	hr = DWriteCreateFactory(
 	//		DWRITE_FACTORY_TYPE_SHARED,
 	//		__uuidof(IDWriteFactory),
 	//		reinterpret_cast<IUnknown**>(&pDWriteFactory_)
 	//	);
 	//}
 	//wszText_ = L"Hello World using  DirectWrite!";
 	//cTextLength_ = (UINT32)wcslen(wszText_);
 	////if (SUCCEEDED(hr))
 	//{
 	//	hr = pDWriteFactory_->CreateTextFormat(
 	//		L"Gabriola",                // Font family name.
 	//		NULL,                       // Font collection (NULL sets it to use the system font collection).
 	//		DWRITE_FONT_WEIGHT_REGULAR,
 	//		DWRITE_FONT_STYLE_NORMAL,
 	//		DWRITE_FONT_STRETCH_NORMAL,
 	//		72.0f,
 	//		L"en-us",
 	//		&pTextFormat_
 	//	);
 	//}
 	//// Center align (horizontally) the text.
 	////if (SUCCEEDED(hr))
 	//{
 	//	hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
 	//}
 	////if (SUCCEEDED(hr))
 	//{
 	//	hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
 	//}
 	//RECT rc;
 	//GetClientRect(hWnd, &rc);
 	//D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
 	////if (!pRT)
 	//{
 	//	// Create a Direct2D render target.
 	//	hr = pD2DFactory_0->CreateHwndRenderTarget(
 	//		D2D1::RenderTargetProperties(),
 	//		D2D1::HwndRenderTargetProperties(
 	//			hWnd,
 	//			size
 	//		),
 	//		&pRT
 	//	);
 	//	// Create a black brush.
 	//	//if (SUCCEEDED(hr))
 	//	{
 	//		hr = pRT->CreateSolidColorBrush(
 	//			D2D1::ColorF(D2D1::ColorF::Black),
 	//			&pBlackBrush_
 	//		);
 	//	}
 	//}
 	//layoutRect = D2D1::RectF(
 	//	static_cast<FLOAT>(rc.left) / 0.5f,
 	//	static_cast<FLOAT>(rc.top) / 0.5f,
 	//	static_cast<FLOAT>(rc.right - rc.left) / 0.5f,
 	//	static_cast<FLOAT>(rc.bottom - rc.top) / 0.5f
 	//);
	

	//directSound 생성

	//// Create the sound object.
	//mspSound = make_unique<SoundClass>();
	//if (!mspSound)
	//{
	//	MessageBox(hWnd, L"Could not Create Direct Sound.", L"Error", MB_OK);
	//}
	//// Initialize the sound object.
	//if (!mspSound->Initialize(hWnd))
	//{
	//	MessageBox(hWnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
	//}

	//fmod 사운드 생성
	//FMOD_System_Create(&m_pSystem); // FMOD system 객체 생성
	//FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL); // FMOD system 초기화
	//MFramework::GameLoop();

	// // 사운드 생성
	//FMOD_SOUND* pSound = NULL;
	//FMOD_System_CreateSound(MFramework::m_pSystem, "Sound\\02-AudioTrack 02.mp3", FMOD_DEFAULT, 0, &pSound);
	//MFramework::m_vecSound.push_back(pSound);
	//// 채널 생성
	//MFramework::m_vecChannel.resize(2);

	//// 사운드 재생
	//FMOD_System_PlaySound(MFramework::m_pSystem, pSound, 0, 0, &MFramework::m_vecChannel[0]);

	//pSound = NULL;
	//FMOD_System_CreateSound(MFramework::m_pSystem, "Sound/Se_m_07.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &pSound);
	//MFramework::m_vecSound.push_back(pSound);

	OnResize();
}

void MFramework::ClearD3D()
{
	MActorManager::Instance().Release();
	ResourceManager::Instance().Release();
	SceneMgr::Instance().Release();
	mspSwapchain->SetFullscreenState(FALSE, NULL);

	// Release the sound object.
	if (mspSound)
	{
		mspSound->Shutdown();
		mspSound.reset();
		mspSound = nullptr;
	}
	//SafeRelease(&pRT);
	//SafeRelease(&pBlackBrush_);

	//// 사운드 해제
	//for (auto p : m_vecSound)
	//{
	//	FMOD_Sound_Release(p);
	//}

	//// 사운드 시스템 종료
	//FMOD_System_Close(m_pSystem);

	//// 사운드 시스템 해제
	//FMOD_System_Release(m_pSystem);
}

void MFramework::Update(float dt)
{
	//MActorManager::Instance().Update(dt);
	SceneMgr::Instance().Update(dt);

}

void MFramework::BeginRender()
{
	mspDeviceCon->ClearRenderTargetView(mspTargetView.Get(), Colors::LawnGreen);
	mspDeviceCon->ClearDepthStencilView(mspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void MFramework::Render()
{

	//mspSpriteBatch->Begin(SpriteSortMode_Deferred, mspStates->Opaque(), mspStates->LinearWrap());
	mspSpriteBatch->Begin(SpriteSortMode_Deferred, mspStates->NonPremultiplied());

	SceneMgr::Instance().Render();

	
	//pRT->BeginDraw();
	//pRT->SetTransform(D2D1::IdentityMatrix());
	//pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	//layoutRect.left = 0;
	//layoutRect.top = 0;
	//layoutRect.right = 500;
	//layoutRect.bottom = 500;
	//pRT->DrawText(
	//	wszText_,        // The string to render.
	//	cTextLength_,    // The string's length.
	//	pTextFormat_,    // The text format.
	//	layoutRect,       // The region of the window where the text will be rendered.
	//	pBlackBrush_     // The brush used to draw the text.
	//);

	//pRT->EndDraw();

	mspSpriteBatch->End();


	//MActorManager::Instance().Draw();
	//RECT sre = { 0, 0 , 100 , 200 };
	//mspspriteFont->DrawString(mspSpriteBatch.get(), sre, XMFLOAT2(100, 100));




}

void MFramework::EndRender()
{
	mspSwapchain->Present(0, 0);
}

void MFramework::OnResize()
{
	/////창크기변환시 케릭터가 사라짐 이것을 방지하기위해 추가한코드
	//NULL은 정수형 0이 되고 nullptr은 비어있는 포인터
	//ID3D11RenderTargetView* nullViews[] = {  };와 같음 
	ID3D11RenderTargetView* nullViews[] = { nullptr };

	mspDeviceCon->OMSetRenderTargets(_countof(nullViews),nullViews,nullptr);
	mspTargetView.Reset();			//스마트 포인터라서 
	mspDepthStencilView.Reset();	//초기화 과정
	mspDeviceCon->Flush();			//변기물 내리기 즉 쌓여있는것을 해결해라
	///////////////////////////////////////// 이과정을 해줘야 빈것에다가 만들어줌 그래야 기존에 있던것이 안남고 이어져나옴
	mspSwapchain->ResizeBuffers(1, mScreenWidth, mScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	mspSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());

	mspDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, mspTargetView.ReleaseAndGetAddressOf());

	D3D11_TEXTURE2D_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));
	dsd.Width = mScreenWidth;
	dsd.Height = mScreenHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = 4;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	mspDevice->CreateTexture2D(&dsd, 0, mspDepthStencilBuffer.ReleaseAndGetAddressOf());
	mspDevice->CreateDepthStencilView(mspDepthStencilBuffer.Get(), 0, mspDepthStencilView.ReleaseAndGetAddressOf());

	mspDeviceCon->OMSetRenderTargets(1, mspTargetView.GetAddressOf(), mspDepthStencilView.Get());

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(mScreenWidth);
	viewport.Height = static_cast<FLOAT>(mScreenHeight);

	mspDeviceCon->RSSetViewports(1, &viewport);
}

void MFramework::InitWindow(HINSTANCE hInstance, LPCTSTR title, UINT width, UINT height)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"JoJo";
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(hInstance, (LPCTSTR)IDC_CURSOR3);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = GlobalWindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, static_cast<LONG>(mScreenWidth), static_cast<LONG>(mScreenHeight) };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	mTitleCaption = title;
	mHwnd = CreateWindowEx(NULL,
		L"JoJo",
		mTitleCaption.c_str(),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(mHwnd, SW_SHOW);

	InitD3D(mHwnd);
}

LRESULT MFramework::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				mPaused = true;
				mTimer.Stop();
			}
			else
			{
				mPaused = false;
				mTimer.Resume();
			}
		break;

		case WM_SIZE:
			mScreenWidth = LOWORD(lParam);
			mScreenHeight = HIWORD(lParam);

			if (mspDevice)
			{
				if (wParam == SIZE_MINIMIZED)
				{
					mPaused = true;
					mMinimized = true;
					mMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					mPaused = false;
					mMinimized = false;
					mMaximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (mMinimized)
					{
						mPaused = false;
						mMinimized = false;
						OnResize();
					}
					else if (mMaximized)
					{
						mPaused = false;
						mMaximized = false;
						OnResize();
					}
					else if (mResizing)
					{

					}
					else
					{
						OnResize();
					}
				}
			}

			return 0;
			break;

		case WM_ENTERSIZEMOVE:
			mPaused = true;
			mResizing = true;
			mTimer.Stop();
			return 0;
			break;

		case WM_EXITSIZEMOVE:
			mPaused = false;
			mResizing = false;
			mTimer.Resume();
			OnResize();
			return 0;
			break;

		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
			break;

		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = STARTSCENE_SCERRN_WIDTH;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = STARTSCENE_SCERRN_HEIGHT;
			return 0;
			break;

		case WM_ACTIVATEAPP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			MFramework::mKeyboardTracker.Reset();
			MFramework::mMouseTracker.Reset();
			break;

		case WM_INPUT:
		case WM_MOUSEMOVE:
			//break;
		case WM_LBUTTONDOWN:
		{
			
			//FMOD_System_PlaySound(m_pSystem, m_vecSound[1], 0, 0, &m_vecChannel[1]);

			//FMOD_Channel_SetPaused(m_vecChannel[0],true);
		}
		//break;
		case WM_LBUTTONUP:
		{
			//FMOD_Channel_SetPaused(m_vecChannel[0], false);

			//FMOD_System_PlaySound(m_pSystem, m_vecSound[0], 0, 0, &m_vecChannel[0]);
		}
		//break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int MFramework::GameLoop()
{
	mTimer.Start();

	MSG msg = { 0 };
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			mTimer.Update();

			if (!mPaused)
			{
				CalculateFPS();

				MFramework::mKeyboardTracker.Update(mspKeyboard->GetState());
				MFramework::mMouseTracker.Update(mspMouse->GetState());

				Update(mTimer.DeltaTime());
				BeginRender();
				//MFramework::Render();
				Render();

				EndRender();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	ClearD3D();

	return msg.wParam;
}

void MFramework::CalculateFPS()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mTitleCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mHwnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}