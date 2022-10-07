#include "Window.h"
#include "DirectX.h"
#include "Input.h"
#include "GPipeline.h"
#include "MyMath.h"
#include "Object3D.h"
#include "VertBuff.h"
#include "ConstBuff.h"
#include "TextureData.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Object2D.h"
#include "DrawSphere.h"
#include "Square.h"
#include "MyXAudio.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	Window win;
	MyDirectX dx(win.hwnd);

	MyDebugCamera debugcamera(Vector3D(0.0f, 300.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	MyXAudio xAudio;
	
	Input input(win.hwnd, win.w);

	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");
	//	描画初期化

	//	定数バッファ
	ConstBuff cBuff(dx.Dev(), window_width, window_height);

#pragma region OrthoProjection
	Square screen(dx.Dev(), bilShader);
	screen.trans.z = 0.1f;
	screen.scale = { window_width / 2,window_height / 2,0.2f };
#pragma endregion
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(window_width, window_height, MyMath::ConvertToRad(45.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(window_width, window_height, 0.1f, 1000.0f);
#pragma endregion Initialize

	//	ゲームループ
	while (true)
	{
#pragma region  WinMsg
		win.MsgUpdate();
		if (win.EndLoop()) { break; }
#pragma endregion

		input.Update(win.hwnd);

#pragma region Update

#pragma endregion

#pragma region Draw
#pragma region ScreenDraw
		dx.DrawAbleScreenTexture();

		// 描画コマンド

		// 描画コマンド

		dx.DrawEndScreenTexture();
#pragma endregion

#pragma region Draw
		dx.DrawAble();

		screen.Draw(dx.CmdList(), dx.TextureHandle(0));

		dx.DrawEnd();
#pragma endregion
#pragma endregion Draw
	}

	return 0;
}