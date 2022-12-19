#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "ScreenPolygon.h"
#include "GameScene.h"

class SceneManager
{
private:
	MyDirectX* dx = MyDirectX::GetInstance();
	IScene* scene = new GameScene();
	IScene* nextScene = nullptr;

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

