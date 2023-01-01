#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "Input.h"
#include "Shader.h"
#include "Model.h"
#include "AssimpModel.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Square.h"

#include "Player.h"
#include "ICamera.h"

class CollisionManager;

class GameScene :public IScene
{
private:
	ICamera* camera = nullptr;
	
	Shader objShader;
	std::unique_ptr<GPipeline> modelpipeline;

	CollisionManager* collisionMan = nullptr;

#pragma region Model
	std::unique_ptr<Model> modelSkydome;
	std::unique_ptr<Model> modelGround;
	std::unique_ptr<Model> modelSword;
	std::unique_ptr<Model> modelSphere;
	std::unique_ptr<AssimpModel> fbxModel;
#pragma endregion

#pragma region Sprite
	std::unique_ptr<Sprite> sprite;

	Square* square = nullptr;
#pragma endregion

#pragma region Texture
	int reimuG = -1;
#pragma endregion
	std::unique_ptr<Object3D> skydome;
	std::unique_ptr<Object3D> ground;
	std::unique_ptr<Object3D> sphere;
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

