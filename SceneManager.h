#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "ScreenPolygon.h"
#include "SceneFactory.h"
#include "ImGuiManager.h"

class SceneManager
{
private:
	MyDirectX* dx = MyDirectX::GetInstance();
	IScene* scene = nullptr;
	IScene* nextScene = nullptr;
	AbstractSceneFactory* sceneFactry = new SceneFactory();
	ImGuiManager* imguiMan = nullptr;

	ScreenPolygon screen;

	SceneManager() {};
	~SceneManager();
public:
	static SceneManager* GetInstance();
	static void DeleteInstance();
	SceneManager(const SceneManager& obj) = delete;
	SceneManager& operator=(const SceneManager& obj) = delete;

	void Initialize();
	void Update();
	void Draw();

	void SetNextScene(IScene* nextScene_);
};

