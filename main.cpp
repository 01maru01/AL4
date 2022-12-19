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

#include "Sprite.h"
#include "SpriteCommon.h"
#include "AssimpModel.h"

#include "SceneManager.h"
#include "ScreenPolygon.h"
#include "Light.h"

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

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	Light* light = nullptr;
	light = light->Create();
	light->SetLightColor({ 1,1,1 });

	Shader shader(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	//Shader bilShader(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");
	Shader objShader(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");
	//	描画初期化
	SceneManager sceneMan;
	sceneMan.Initialize();
#pragma region OrthoProjection
	ScreenPolygon screen;
#pragma endregion
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(45.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);
#pragma endregion Initialize

	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));
	std::unique_ptr<GPipeline> modelpipeline(new GPipeline(dx->GetDev(), objShader));

	Model::SetLight(light);

	Model ground(dx , shader, "ground", modelpipeline.get());
	Model skydome(dx , shader, "skydome", modelpipeline.get());
	Model cat(dx, objShader, "12161_Cat_v1_L2", modelpipeline.get());
	cat.mat.scale = { 0.1f,0.1f,0.1f };
	cat.mat.trans.x = 2.0f;
	cat.mat.rotAngle.x = -MyMath::PI / 2.0f;
	cat.mat.rotAngle.y = MyMath::PI / 2.0f;
	Model skull(dx , objShader, "12140_Skull_v3_L2", modelpipeline.get());
	skull.mat.scale = { 0.1f,0.1f,0.1f };
	skull.mat.rotAngle.y = MyMath::PI;
	skull.mat.trans.x = -2.0f;

	Model sphere(dx, objShader, "sphere", modelpipeline.get(), true);
	sphere.mat.trans.x = -1.0f;
	sphere.mat.trans.y = 1.0f;
	Model sword(dx, objShader, "chr_sword", modelpipeline.get());
	sword.mat.trans.x = 1.0f;
	sword.mat.rotAngle.y = MyMath::PI;

	std::unique_ptr<Sprite> sprite(new Sprite);
	sprite->Initialize();

	AssimpModel fbxModel(modelpipeline.get());
	fbxModel.Initialize(L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX");
	//	ゲームループ
	while (true)
	{
#pragma region  WinMsg
		if (win->MsgUpdate() || input->GetTrigger(DIK_ESCAPE)) { break; }
#pragma endregion
		input->Update();

#pragma region Update
		//sceneMan.Update();
		light->Update();

		debugcamera.Update();
		sphere.mat.rotAngle.y += 0.1f;
		
		ground.MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);
		skydome.MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);
		sphere.MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);
		sword.MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);
		fbxModel.MatUpdate(debugcamera.mat, matProjection);

		sprite->MatUpdate();
#pragma endregion

#pragma region Draw
		//sceneMan.Draw();
#pragma region ScreenDraw
		dx->PrevDrawScreen();

		// 描画コマンド
		ground.Draw();
		skydome.Draw();

		sphere.Draw();
		sword.Draw();

		// 描画コマンド

		dx->PostDrawScreen();
#pragma endregion

#pragma region UIDraw
		dx->PrevDraw();

		screen.Draw();

		dx->PostDraw();
#pragma endregion
#pragma endregion Draw
	}
	SpriteCommon::DeleteInstance();
	Input::DeleteInstance();
	MyDirectX::DeleteInstance();
	Window::DeleteInstance();

	return 0;
}