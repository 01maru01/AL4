#include "Window.h"
#include "DirectX.h"
#include "Input.h"
#include "MyXAudio.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "Light.h"

#include "AssimpModel.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	Window* win = Window::GetInstance();
	win->Initialize();

	MyDirectX* dx = MyDirectX::GetInstance();
	dx->Initialize();

	MyXAudio* xAudio = MyXAudio::GetInstance();

	Input* input = Input::GetInstance();

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	Light* light = nullptr;
	light = light->Create();
	light->SetLightDir({ 0,0,1 });
	light->SetLightColor({ 1,1,1 });

	SceneManager sceneMan;
	sceneMan.Initialize();
	
	//Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	Model::SetLight(light);

	//AssimpModel fbxModel(modelpipeline.get());
	//fbxModel.Initialize(L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX");
	
	//	ƒQ[ƒ€ƒ‹[ƒv
	while (true)
	{
#pragma region  WinMsg
		if (win->MsgUpdate() || input->GetTrigger(DIK_ESCAPE)) { break; }
#pragma endregion

#pragma region Update
		input->Update();
		light->Update();
		sceneMan.Update();
#pragma endregion

#pragma region Draw
		sceneMan.Draw();
#pragma endregion Draw
	}
	SpriteCommon::DeleteInstance();
	Input::DeleteInstance();
	MyXAudio::DeleteInstance();
	MyDirectX::DeleteInstance();
	Window::DeleteInstance();

	return 0;
}