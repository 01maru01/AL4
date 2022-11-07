#include "Window.h"
#pragma comment(lib,"winmm.lib")

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window* Window::GetInstance()
{
	static Window* instance = new Window;
	return instance;
}

void Window::Destroy()
{
	delete Window::GetInstance();
}

Window::Window()
{
}

Window::~Window()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

void Window::Initialize()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame";			// ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);		// ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル指定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,	// クラス名
		L"DirectXGame",						// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,				// 標準的なウィンドウスタイル
		CW_USEDEFAULT,						// 表示X座標（OSに任せる）
		CW_USEDEFAULT,						// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,				// ウィンドウ横幅
		wrc.bottom - wrc.top,				// ウィンドウ縦幅
		nullptr,							// 親ウィンドウハンドル
		nullptr,							// メニューハンドル
		w.hInstance,						// 呼び出しアプリケーションハンドル
		nullptr);							// オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	timeBeginPeriod(1);
}

void Window::MsgUpdate()
{
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);			// キー入力メッセージの処理
		DispatchMessage(&msg);			// プロシージャにメッセージを送る
	}
}

bool Window::EndLoop()
{
	if (msg.message == WM_QUIT) return true;
	return false;
}
