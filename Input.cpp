#include "Input.h"
#include "Window.h"

Input::Input()
{
	Initialize();
}

Input* Input::GetInstance()
{
	static Input* instance = new Input;
	return instance;
}

void Input::DeleteInstance()
{
	delete Input::GetInstance();
}

void Input::Initialize()
{
	Window* win = Window::GetInstance();

	//inputHwnd = win->GetHwnd();
	//	DirectInput初期化
	HRESULT result = DirectInput8Create(
		win->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//	デバイス生成(キーボード以外も可能)
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = keyboard->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));


	result = DirectInput8Create(
		win->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//	デバイス生成(キーボード以外も可能)
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = mouse->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//	前フレームの情報取得
	for (size_t i = 0; i < sizeof(key); i++)
	{
		prev[i] = key[i];
	}

	prevclick = click;

	//	キー情報取得
	keyboard->Acquire();
	//	全キーの入力情報取得
	keyboard->GetDeviceState(sizeof(key), key);

	mouse->Acquire();
	mouse->Poll();
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &click);
	GetCursorPos(&cursor);
	ScreenToClient(Window::GetInstance()->GetHwnd(), &cursor);
}

bool Input::GetKey(int _key)
{
	return key[_key];
}

bool Input::GetTrigger(int _key)
{
	return key[_key] && !prev[_key];
}

bool Input::ReleaseKey(int _key)
{
	return prev[_key] && !key[_key];
}

bool Input::Click(int type)
{
	return (click.rgbButtons[type] & (0x80));
	
}

bool Input::ClickTrriger(int type)
{
	return (click.rgbButtons[type] & (0x80)) && !(prevclick.rgbButtons[type] & (0x80));
}

Vector2D Input::CursorPos()
{
	Vector2D ans(cursor.x, cursor.y);
	return ans;
}

void Input::CursorPos(Vector2D& pos)
{
	pos.x = cursor.x;
	pos.y = cursor.y;
}

LONG Input::Wheel()
{
	return click.lZ;
}
