#pragma once
#include "IScene.h"
#include "DirectX.h"

class SceneManager
{
private:
	MyDirectX* dx = MyDirectX::GetInstance();
	IScene* scene = nullptr;
public:
	void Initialize();
	void Update();
	void Draw();
};

