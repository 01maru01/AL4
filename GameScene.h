#pragma once
#include "Input.h"
#include "DirectX.h"
class GameScene
{
private:
	MyDirectX* dx = nullptr;
	Input* input = Input::GetInstance();
public:
	GameScene();
	void Initialize(MyDirectX* dx_, Input* input_);
	void Update();
	void Draw();
	void DrawMultiPath();
};

