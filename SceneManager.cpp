#include "SceneManager.h"

SceneManager::~SceneManager()
{
	scene->Finalize();
	delete scene;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager* instance = new SceneManager;
	return instance;
}

void SceneManager::DeleteInstance()
{
	delete SceneManager::GetInstance();
}

void SceneManager::Initialize()
{
	scene = sceneFactry->CreateScene("GAMESCENE");
	scene->Initialize();
}

void SceneManager::Update()
{
	if (nextScene) {
		if (scene) {
			scene->Finalize();
			delete scene;
		}

		scene = nextScene;
		nextScene = nullptr;
		scene->Initialize();
	}
	scene->Update();
}

void SceneManager::Draw()
{
#pragma region DrawScreen
	dx->PrevDrawScreen();

	scene->Draw();

	dx->PostDrawScreen();
#pragma endregion

#pragma region MultiPath
	dx->PrevDraw();

	screen.Draw();

	dx->PostDraw();
#pragma endregion
}

void SceneManager::SetNextScene(IScene* nextScene_)
{
	nextScene = nextScene_;
}
