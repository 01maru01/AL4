#pragma once
#include "Input.h"
#include "DirectX.h"
#include "IScene.h"
class GameScene :public IScene
{
private:
	
public:
	GameScene();
	~GameScene() override;
	void Initialize() override;
	void LoadResources() override;
	void Update() override;
	void Draw() override;
private:
	MyDirectX* dx = MyDirectX::GetInstance();
	Input* input = Input::GetInstance();

	void MatUpdate() override;
};

