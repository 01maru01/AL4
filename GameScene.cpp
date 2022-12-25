#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

void GameScene::MatUpdate()
{
	ground->MatUpdate();
	skydome->MatUpdate();
	player->MatUpdate();
	sphere->MatUpdate();

	sprite->MatUpdate();
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;
}

void GameScene::Initialize()
{
	collisionMan = CollisionManager::GetInstance();

	camera = new GameCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	objShader.Initialize(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");

	modelpipeline = std::make_unique<GPipeline>(dx->GetDev(), objShader);

	Object3D::SetPipeline(modelpipeline.get());
	Object3D::SetCamera(camera);
	LoadResources();

	sphere->SetCollider(new SphereCollider());
	sphere->SetPosition(Vector3D(3.0f, 1.0f, 0.0f));
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
	player->PlayerInitialize(modelSword.get());
}

void GameScene::Finalize()
{
}

void GameScene::LoadResources()
{
#pragma region Model
	modelSword = std::make_unique<Model>("chr_sword");
	modelSkydome = std::make_unique<Model>("skydome");
	modelGround = std::make_unique<Model>("ground");
	modelSphere = std::make_unique<Model>("sphere");
#pragma endregion

#pragma region AssimpModel
	//fbxModel = std::make_unique<AssimpModel>(modelpipeline.get());
	//fbxModel->Initialize(L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX");
#pragma endregion
	skydome.reset(Object3D::Create(modelSkydome.get()));
	ground.reset(Object3D::Create(modelGround.get()));
	sphere.reset(Object3D::Create(modelSphere.get()));
#pragma region Sprite
	sprite = std::make_unique<Sprite>();
#pragma endregion

#pragma region Texture
	reimuG = dx->LoadTextureGraph(L"Resources/reimu.png");
#pragma endregion
}

void GameScene::Update()
{
#pragma region XVˆ—
	camera->Update();

	player->Update();
	sphere->ColliderUpdate();
#pragma endregion
	collisionMan->CheckAllCollisions();

	MatUpdate();
}

void GameScene::Draw()
{
	ground->Draw();
	skydome->Draw();
	sphere->Draw();
	player->Draw();

	sprite->Draw(reimuG);
}
