#include "stdafx.h"
#include <sstream>

SGAFramework* gpDispatch = 0;
 DirectX::Mouse::ButtonStateTracker SGAFramework::mMouseTracker;
 DirectX::Keyboard::KeyboardStateTracker SGAFramework::mKeyboardTracker;
using namespace std;

LRESULT CALLBACK GlobalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return gpDispatch->WindowProc(hWnd, message, wParam, lParam);
}

SGAFramework::SGAFramework() : 
	mScreenWidth(DEFAULT_SCREEN_WIDTH),
	mScreenHeight(DEFAULT_SCREEN_HEIGHT),
	mPaused(false),
	mMinimized(false),
	mMaximized(false),
	mResizing(false)
{
	gpDispatch = this;
}

SGAFramework::~SGAFramework()
{
}

void SGAFramework::InitD3D(HWND hWnd)
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

	SGAResourceManager::Instance().Init(hWnd, mspDevice.Get());

	mspKeyboard = make_unique<DirectX::Keyboard>();
	mspMouse = make_unique<DirectX::Mouse>();
	mspMouse->SetWindow(hWnd);

	mspSpriteBatch = make_unique<SpriteBatch>(mspDeviceCon.Get());
	//(new SpriteFont(device, L"myfile.spritefont"));
	mspspriteFont = make_unique<SpriteFont>(mspDevice.Get(), L"myfile.spritefont");

	mspStates = std::make_unique<CommonStates>(mspDevice.Get());

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
	OnResize();
}

void SGAFramework::ClearD3D()
{
	SGAActorManager::Instance().Release();
	SGAResourceManager::Instance().Release();

	mspSwapchain->SetFullscreenState(FALSE, NULL);
}

void SGAFramework::Update(float dt)
{
	//SGAActorManager::Instance().Update(dt);
	SceneMgr::Instance().Update(dt);

}

void SGAFramework::BeginRender()
{
	mspDeviceCon->ClearRenderTargetView(mspTargetView.Get(), Colors::LawnGreen);
	mspDeviceCon->ClearDepthStencilView(mspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void SGAFramework::Render()
{
	//mspSpriteBatch->Begin(SpriteSortMode_Deferred, mspStates->Opaque(), mspStates->LinearWrap());
	mspSpriteBatch->Begin();

	SceneMgr::Instance().Render();
	//SGAActorManager::Instance().Draw();
	RECT sre = { 0, 0 , 100 , 200 };
	//mspspriteFont->DrawString(mspSpriteBatch.get(), sre, XMFLOAT2(100, 100));

	mspSpriteBatch->End();
}

void SGAFramework::EndRender()
{
	mspSwapchain->Present(0, 0);
}

void SGAFramework::OnResize()
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

void SGAFramework::InitWindow(HINSTANCE hInstance, LPCTSTR title, UINT width, UINT height)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"SGAWindowClass";
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = GlobalWindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, static_cast<LONG>(mScreenWidth), static_cast<LONG>(mScreenHeight) };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	mTitleCaption = title;
	mHwnd = CreateWindowEx(NULL,
		L"SGAWindowClass",
		mTitleCaption.c_str(),
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(mHwnd, SW_SHOW);

	InitD3D(mHwnd);
}

LRESULT SGAFramework::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
						OnResize();
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
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
			return 0;
			break;

		case WM_ACTIVATEAPP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			SGAFramework::mKeyboardTracker.Reset();
			SGAFramework::mMouseTracker.Reset();
			break;

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
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

int SGAFramework::GameLoop()
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

				SGAFramework::mKeyboardTracker.Update(mspKeyboard->GetState());
				SGAFramework::mMouseTracker.Update(mspMouse->GetState());

				Update(mTimer.DeltaTime());
				BeginRender();
				SGAFramework::Render();
				//Render();
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

void SGAFramework::CalculateFPS()
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