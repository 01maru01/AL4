#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include "Vector2D.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input
{
public:
	enum MouseButton {
		LeftClick = 0,
		RightClick,
		WheelClick,
	};
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	BYTE key[256] = {};
	BYTE prev[256] = {};
	ComPtr<IDirectInputDevice8> keyboard = nullptr;
	ComPtr<IDirectInput8> directInput = nullptr;

	DIMOUSESTATE click = {};
	DIMOUSESTATE prevclick = {};
	ComPtr<IDirectInputDevice8> mouse = nullptr;
	Vector2D cursor;
	Vector2D prevCursor;
	bool rockCursor = false;

	Input();
	~Input() {};
public:
	static Input* GetInstance();
	static void DeleteInstance();
	Input(const Input& obj) = delete;
	Input& operator=(const Input& obj) = delete;

	void Initialize();
	void Update();

	bool GetKey(int _key);
	bool GetTrigger(int _key);
	bool ReleaseKey(int _key);

	bool Click(int type);
	bool ClickTrriger(int type);

	Vector2D GetCursor() { return cursor; }
	Vector2D GetPrevCursor() { return prevCursor; }
	void RockCursor(bool isRock) { rockCursor = isRock; }
	LONG Wheel();
};

