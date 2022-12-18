#include "SceneManager.h"

void SceneManager::Initialize()
{
	scene->Initialize();
}

void SceneManager::Update()
{
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
