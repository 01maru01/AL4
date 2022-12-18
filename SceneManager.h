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

	ScreenPolygon screen;
public:
	void Initialize();
	void Update();
	void Draw();
};

