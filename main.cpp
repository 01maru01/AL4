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
#include "Model.h"
#include "GameScene.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	Window* win = Window::GetInstance();
	win->Initialize();

	MyDirectX* dx = MyDirectX::GetInstance();
	dx->Initialize();
	int reimu = dx->LoadTextureGraph(L"Resources/reimu.png");

	MyDebugCamera debugcamera(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	MyXAudio xAudio;
	
	Input* input = Input::GetInstance();
	input->Initialize();

	Shader shader(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	Shader bilShader(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");
	Shader objShader(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");
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

	//std::unique_ptr<GameScene> gamescene(new GameScene());
	//gamescene->Initialize(dx.get(), input.get());

	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));
	std::unique_ptr<GPipeline> modelpipeline(new GPipeline(dx->GetDev(), objShader));
	Object3D obj(dx, pipeline.get(), shader);
	//Model box(dx , shader, "Resource\\Model\\box.obj", pipeline.get());
	Model ground(dx , shader, "ground", modelpipeline.get());
	Model skydome(dx , shader, "skydome", modelpipeline.get());
	Model airplane(dx , shader, "MiG-25PD", modelpipeline.get());
	//	ゲームループ
	while (true)
	{
#pragma region  WinMsg
		if (win->MsgUpdate() || input->GetTrigger(DIK_ESCAPE)) { break; }
#pragma endregion

		//gamescene->Update();
		input->Update();

#pragma region Update
		debugcamera.Update();
		screen.MatUpdate(matView.mat, orthoProjection);
		
		//box.mat.trans = debugcamera.target;
		obj.MatUpdate(debugcamera.mat, matProjection);
		ground.MatUpdate(debugcamera.mat, matProjection);
		skydome.MatUpdate(debugcamera.mat, matProjection);
#pragma endregion

#pragma region Draw
#pragma region ScreenDraw
		dx->PrevDrawScreen();

		//gamescene->Draw();
		// 描画コマンド
		obj.Draw(dx->GetCmdList(), dx->GetTextureHandle(reimu));
		airplane.Draw();
		ground.Draw();
		skydome.Draw();
		// 描画コマンド

		dx->PostDrawScreen();
#pragma endregion

#pragma region UIDraw
		dx->PrevDraw();

		//gamescene->DrawMultiPath();
		screen.Draw(dx->GetCmdList(), dx->GetTextureHandle(0));

		dx->PostDraw();
#pragma endregion
#pragma endregion Draw
	}
	Window::Destroy();
	MyDirectX::Destroy();
	Input::Destroy();

	return 0;
}