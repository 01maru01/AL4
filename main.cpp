#include "Window.h"
#include "DirectX.h"
#include "Input.h"
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "TextureData.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "MyXAudio.h"
#include <memory>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	std::unique_ptr<Window> win(new Window());

	std::unique_ptr<MyDirectX> dx(new MyDirectX(win.get()));
	int reimu = dx->LoadTextureGraph(L"Resource/reimu.png");

	MyDebugCamera debugcamera(Vector3D(0.0f, 300.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	MyXAudio xAudio;
	
	std::unique_ptr<Input> input(new Input(win.get()));

	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");
	//	描画初期化

	//	定数バッファ
	ConstBuff cBuff(dx->GetDev(), Window::window_width, Window::window_height);

#pragma region OrthoProjection
	Square screen(dx->GetDev(), bilShader);
	screen.trans.z = 0.1f;
	screen.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };
#pragma endregion
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(45.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);
#pragma endregion Initialize

	Object3D obj(dx->GetDev(), shader);
	//	ゲームループ
	while (true)
	{
#pragma region  WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }
#pragma endregion

		input->Update();

#pragma region Update
		debugcamera.Update(*input);
		screen.MatUpdate(matView.mat, orthoProjection);
		
		obj.trans = debugcamera.target;
		obj.MatUpdate(matView.mat, matProjection);
#pragma endregion

#pragma region Draw
#pragma region ScreenDraw
		dx->PrevDrawScreen();

		// 描画コマンド
		obj.Draw(dx->GetCmdList(), dx->GetTextureHandle(reimu));
		// 描画コマンド

		dx->PostDrawScreen();
#pragma endregion

#pragma region UIDraw
		dx->PrevDraw();

		screen.Draw(dx->GetCmdList(), dx->GetTextureHandle(0));

		dx->PostDraw();
#pragma endregion
#pragma endregion Draw
	}
	return 0;
}