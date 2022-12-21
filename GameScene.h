#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "Input.h"
#include "MyDebugCamera.h"
#include "Shader.h"
#include "Model.h"
#include "AssimpModel.h"
#include "Sprite.h"

#include "Player.h"
#include "ICamera.h"

class GameScene :public IScene
{
private:
	ICamera* camera = nullptr;
	
	Shader objShader;
	std::unique_ptr<GPipeline> modelpipeline;

	MyMath::MatView matView;
	Matrix matProjection;

#pragma region Model
	std::unique_ptr<Model> skydome;
	std::unique_ptr<Model> ground;
	std::unique_ptr<Model> sphere;
	std::unique_ptr<Model> sword;
	std::unique_ptr<AssimpModel> fbxModel;
#pragma endregion

#pragma region Sprite
	std::unique_ptr<Sprite> sprite;
#pragma endregion

#pragma region Texture
	int reimuG;
#pragma endregion

	std::unique_ptr<Player> player;

public:
	GameScene();
	~GameScene() override;
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void Draw() override;
private:
	MyDirectX* dx = MyDirectX::GetInstance();
	Input* input = Input::GetInstance();

	void MatUpdate() override;
};

