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
	//	DirectInput������
	HRESULT result = DirectInput8Create(
		win->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//	�f�o�C�X����(�L�[�{�[�h�ȊO���\)
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//	���͌`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//	�r������̃��x���Z�b�g
	result = keyboard->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));


	result = DirectInput8Create(
		win->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//	�f�o�C�X����(�L�[�{�[�h�ȊO���\)
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	//	���͌`���̃Z�b�g
	result = mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//	�r������̃��x���Z�b�g
	result = mouse->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//	�O�t���[���̏��擾
	for (size_t i = 0; i < sizeof(key); i++)
	{
		prev[i] = key[i];
	}

	prevclick = click;
	prevCursor = cursor;

	//	�L�[���擾
	keyboard->Acquire();
	//	�S�L�[�̓��͏��擾
	keyboard->GetDeviceState(sizeof(key), key);

	mouse->Acquire();
	mouse->Poll();
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &click);
	POINT cursor_;
	GetCursorPos(&cursor_);

	cursor.x = (float)cursor_.x;
	cursor.y = (float)cursor_.y;

	RECT* rec = new RECT();
	if (rockCursor) {
		GetWindowRect(Window::GetInstance()->GetHwnd(), rec);
		Vector2D center((float)(rec->right + rec->left) / 2, (float)(rec->bottom + rec->top) / 2);
		float width = Window::window_width / 2.0f;
		float height = Window::window_height / 2.0f;
		rec->left = (LONG)(center.x - width);
		rec->right = (LONG)(center.x + width);
		rec->top = (LONG)(center.y - height);
		rec->bottom = (LONG)(center.y + height);
		prevCursor = center;
		SetCursorPos((int)center.x, (int)center.y);
		//	�J�[�\���\�����Ȃ�
		ShowCursor(false);
		//	�͈͎w��
		ClipCursor(rec);
	}
	else {
		ClipCursor(NULL);
		ScreenToClient(Window::GetInstance()->GetHwnd(), &cursor_);
	}
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

//void Input::SetCursorPos(Vector2D& pos)
//{
//	pos = cursor;
//}

LONG Input::Wheel()
{
	return click.lZ;
}
