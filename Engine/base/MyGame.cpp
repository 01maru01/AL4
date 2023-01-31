#include "MyGame.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	light = Light::GetInstance();
	light->Initialize();
	light->SetDirLightActive(0, true);
	//light->SetDirLightActive(1, true);
	//light->SetDirLightActive(2, true);
	//light->SetDirLightDir(0, { 0,0,1 });
	//light->SetDirLightColor(0, { 1,1,1 });
	//light->SetCircleShadowActive(0, true);
	Object3D::SetLight(light);

	light->SetCircleShadowDir(0, { 0.0f,1.0f,0.0f });
	light->SetCircleShadowFactorAngle(0, { 0.0f,0.5f });
	light->SetCircleShadowAtten(0, { 0.5f,0.5f,0.0f });

	light->SetSpotLightPos(0, { 0.0f,5.0f,0.0f });
	light->SetSpotLightFactorAngle(0, { 30.0f,35.0f });
	light->SetSpotLightDir(0, { 0.0f,1.0f,0.0f });

	light->SetPointLightColor(0, { 1.0f,0.0f,1.0f });
	light->SetPointLightPos(0, { 0.5f,1.0f,0.0f });

	light->SetPointLightColor(1, { 1.0f,1.0f,1.0f });
	light->SetPointLightPos(1, { 5.0f,1.0f,0.0f });

	light->SetPointLightColor(2, { 0.0f,1.0f,1.0f });
	light->SetPointLightPos(2, { -3.0f,1.0f,-3.0f });

	sceneMan = SceneManager::GetInstance();
	sceneMan->Initialize();
}

void MyGame::Update()
{
	Framework::Update();

	light->Update();
	sceneMan->Update();
}

void MyGame::Draw()
{
	sceneMan->Draw();
}

void MyGame::Finalize()
{
	Light::DeleteInstance();
	SceneManager::DeleteInstance();
	Framework::Finalize();
}
