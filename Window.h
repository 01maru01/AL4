#pragma once
#include <Windows.h>

class Window
{
public:
	// ウィンドウサイズ
	static const int window_width = 1280; // 横幅
	static const int window_height = 720; // 縦幅
private:
	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	// ウィンドウオブジェクトの生成
	HWND hwnd;
	// メッセージ
	MSG msg{};
public:
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static Window* GetInstance();
	static void Destroy();
	Window();
	~Window();
	void Initialize();
	bool MsgUpdate();
	bool EndLoop();

	//	getter
	HWND GetHwnd() const { return hwnd; }
	WNDCLASSEX GetWND() const { return w;}
};

