#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "SceneManager.h"

void GameScene::CollisionUpdate()
{
	player->CollisionUpdate();
	collisionMan->CheckAllCollisions();
}

void GameScene::MatUpdate()
{
	ground->MatUpdate();
	skydome->MatUpdate();
	player->MatUpdate();
	sphere->MatUpdate();

	square->MatUpdate();
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;

	delete square;
}

void GameScene::Initialize()
{
	collisionMan = CollisionManager::GetInstance();

	camera = new MyDebugCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	objShader.Initialize(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");

	modelpipeline = std::make_unique<GPipeline>(objShader, GPipeline::ALPHA_BLEND, 4);

	Object3D::SetPipeline(modelpipeline.get());
	Object3D::SetCamera(camera);
	LoadResources();

	sphere->SetCollider(new SphereCollider());
	sphere->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	sphere->SetPosition(Vector3D(3.0f, 1.0f, 0.0f));
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
	player->PlayerInitialize(modelSword.get());

	Particle::SetCamera(camera);
	square = new Particle();
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
	fbxModel = std::make_unique<Model>("Alicia/FBX/Alicia_solid_Unity.FBX", true);
#pragma endregion
	skydome.reset(Object3D::Create(modelSkydome.get()));
	ground.reset(TouchableObject::Create(modelGround.get()));
	sphere.reset(Object3D::Create(modelSphere.get()));
#pragma region Texture
	reimuG = dx->LoadTextureGraph(L"Resources/reimu.png");
#pragma endregion

#pragma region Sprite
	sprite = std::make_unique<Sprite>(reimuG);
	sprite->SetSize(Vector2D(200.0f, 200.0f));
	//sprite->SetTextureLeftTop(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
	sprite->SetTextureSize(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
#pragma endregion
}

void GameScene::Update()
{
#pragma region XVˆ—
	if (input->GetTrigger(DIK_1)) {
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}
	camera->Update();

	player->Update();
	ground->ColliderUpdate();
	sphere->ColliderUpdate();

	sprite->Update();
#pragma endregion
	MatUpdate();

	CollisionUpdate();
}

void GameScene::Draw()
{
	ground->Draw();
	skydome->Draw();
	sphere->Draw();
	player->Draw();
	square->Draw(reimuG);

	sprite->Draw();
}
