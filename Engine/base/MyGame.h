#pragma once
#include "SceneManager.h"
#include "Light.h"
#include "Framework.h"

class MyGame :public Framework
{
private:
	SceneManager* sceneMan = nullptr;
	Light* light = nullptr;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};

