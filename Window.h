#pragma once
#include <Windows.h>

// �E�B���h�E�T�C�Y
const int window_width = 1280; // ����
const int window_height = 720; // �c��
class Window
{
public:
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g�̐���
	HWND hwnd;
	// ���b�Z�[�W
	MSG msg{};
public:
	// �E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	Window();
	~Window();
	void MsgUpdate();
	bool EndLoop();
};

