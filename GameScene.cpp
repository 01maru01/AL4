#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"

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

	sprite->MatUpdate();
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

	modelpipeline = std::make_unique<GPipeline>(dx->GetDev(), objShader);

	Object3D::SetPipeline(modelpipeline.get());
	Object3D::SetCamera(camera);
	LoadResources();

	MeshCollider* collider = new MeshCollider;
	collider->ConstructTriangles(ground->GetModel());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	ground->SetCollider(collider);

	MeshCollider* spherecollider = new MeshCollider;
	spherecollider->ConstructTriangles(sphere->GetModel());
	spherecollider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	sphere->SetCollider(spherecollider);
	//sphere->SetAttribute(COLLISION_ATTR_LANDSHAPE);
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
	ground->ColliderUpdate();
	sphere->ColliderUpdate();
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

	sprite->Draw(reimuG);
}
