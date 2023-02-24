#pragma once
#include "IScene.h"
#include "DirectX.h"
#include "Input.h"
#include "Shader.h"
#include "Model.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Object2D.h"
#include "Particle.h"
#include "TouchableObject.h"

#include "Player.h"
#include "ICamera.h"

class CollisionManager;

class GameScene :public IScene
{
private:
	ICamera* camera = nullptr;
	
	Shader objShader;
	std::unique_ptr<GPipeline> modelpipeline;
	std::unique_ptr<GPipeline> obj2Dpipeline;

	CollisionManager* collisionMan = nullptr;

	bool isDebug = true;
#pragma region Model
	std::unique_ptr<Model> modelSkydome;
	std::unique_ptr<Model> modelGround;
	std::unique_ptr<Model> modelSword;
	std::unique_ptr<Model> modelTree;
	std::unique_ptr<Model> modelSmoothSphere;
#pragma endregion

#pragma region Sprite
	std::unique_ptr<Sprite> sprite;

	Particle* square = nullptr;
#pragma endregion
	int bgmSound = -1;
#pragma region Texture
	int reimuG = -1;
	int grassG = -1;
	int lightG = -1;
#pragma endregion
	std::unique_ptr<Object3D> skydome;
	std::unique_ptr<TouchableObject> ground;
	std::unique_ptr<Object3D> tree;
	std::unique_ptr<Object3D> sphere2;
	std::unique_ptr<Player> player;
	std::unique_ptr<Object2D> test2d;

	int mord = 0;

public:
	enum TaskMord
	{
		Phong,
		PointLight,
		SpotLight,
		CircleShadow,
	};
	
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

	void CollisionUpdate();
	void MatUpdate() override;
};

