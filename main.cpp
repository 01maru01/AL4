#include "Window.h"
#include "DirectX.h"
#include "Input.h"
#include "InputJoypad.h"
#include "MyXAudio.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "Light.h"
#include "Object3D.h"
#include "ParticleCommon.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	Window* win = Window::GetInstance();
	win->Initialize();

	MyDirectX* dx = MyDirectX::GetInstance();
	dx->Initialize();

	MyXAudio* xAudio = MyXAudio::GetInstance();

	Input* input = Input::GetInstance();
	InputJoypad* joypad = InputJoypad::GetInstance();

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	ParticleCommon* particleCommon = ParticleCommon::GetInstance();
	particleCommon->Initialize();

	Light* light = nullptr;
	light = light->Create();
	light->SetLightDir({ 0,0,1 });
	light->SetLightColor({ 1,1,1 });
	Object3D::SetLight(light);
	

	SceneManager* sceneMan = SceneManager::GetInstance();
	sceneMan->Initialize();
	//	ゲームループ
	while (true)
	{
#pragma region  WinMsg
		if (win->MsgUpdate() || input->GetTrigger(DIK_ESCAPE)) { break; }
#pragma endregion

#pragma region Update
		input->Update();
		joypad->Update();
		light->Update();
		sceneMan->Update();
#pragma endregion

#pragma region Draw
		sceneMan->Draw();
#pragma endregion Draw
	}
	delete light;
	SceneManager::DeleteInstance();
	ParticleCommon::DeleteInstance();
	SpriteCommon::DeleteInstance();
	InputJoypad::DeleteInstance();
	Input::DeleteInstance();
	MyXAudio::DeleteInstance();
	MyDirectX::DeleteInstance();
	Window::DeleteInstance();

	return 0;
}