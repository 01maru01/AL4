#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"

void GameScene::MatUpdate()
{
	ground->Update();
	skydome->Update();
	sword->Update();
	//player->MatUpdate(camera->GetMatrix(), matProjection, camera->GetEye());

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
	camera = new MyDebugCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	objShader.Initialize(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");

	modelpipeline = std::make_unique<GPipeline>(dx->GetDev(), objShader);

	Object3D::SetPipeline(modelpipeline.get());
	Object3D::SetCamera(camera);
	LoadResources();

	//player = std::make_unique<Player>();
	//player->Initialize("chr_sword", true);
	//player->PlayerInitialize();
	//player->SetCamera(camera);
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
#pragma endregion

#pragma region AssimpModel
	//fbxModel = std::make_unique<AssimpModel>(modelpipeline.get());
	//fbxModel->Initialize(L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX");
#pragma endregion
	sword = Object3D::Create(modelSword.get());
	skydome.reset(Object3D::Create(modelSkydome.get()));
	ground.reset(Object3D::Create(modelGround.get()));
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

	//player->Update();
#pragma endregion
	MatUpdate();
}

void GameScene::Draw()
{
	ground->Draw();
	skydome->Draw();

	sword->Draw();

	//player->Draw();

	sprite->Draw(reimuG);
}
