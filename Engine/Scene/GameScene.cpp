#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "SceneManager.h"
#include "MyXAudio.h"
#include "NormalCamera.h"
#include "PlaneCollider.h"
#include "Collision.h"

void GameScene::CollisionUpdate()
{
	PlaneCollider* planeCollider = dynamic_cast<PlaneCollider*>(ground->GetCollider());
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(sphere->GetCollider());
	if (Collision::CheckSphere2Plane(*sphereCollider, *planeCollider)) {
		sphere->SetColor({ 1.0f,0.2f,0.2f });
	}
	else {
		sphere->SetColor({ 1.0f,1.0f,1.0f });
	}
	SphereCollider* sphere2Collider = dynamic_cast<SphereCollider*>(sphere2->GetCollider());
	if (Collision::CheckSphere2Plane(*sphere2Collider, *planeCollider)) {
		sphere2->SetColor({ 1.0f,0.2f,0.2f });
	}
	else {
		sphere2->SetColor({ 1.0f,1.0f,1.0f });
	}
	
	player->CollisionUpdate();
	collisionMan->CheckAllCollisions();
}

void GameScene::MatUpdate()
{
	ground->MatUpdate();
	skydome->MatUpdate();
	player->MatUpdate();
	sphere->MatUpdate();
	sphere2->MatUpdate();

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

	ground->SetCollider(new PlaneCollider());
	ground->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	sphere->SetCollider(new SphereCollider());
	sphere->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	sphere->SetPosition(Vector3D(3.0f, 1.0f, 0.0f));
	sphere2->SetCollider(new SphereCollider());
	sphere2->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
	player->PlayerInitialize(modelSword.get());

	Particle::SetCamera(camera);
	square = new Particle();

	mord = Phong;

	bgmSound = MyXAudio::GetInstance()->SoundLoadWave("gameBGM.wav");

	MyXAudio::GetInstance()->SoundPlayLoopWave(bgmSound, 0.05f);
}

void GameScene::Finalize()
{
	MyXAudio::GetInstance()->StopAllLoopSound();
}

void GameScene::LoadResources()
{
#pragma region Model
	modelSword = std::make_unique<Model>("chr_sword");
	modelSkydome = std::make_unique<Model>("skydome");
	modelGround = std::make_unique<Model>("ground");
	modelSphere = std::make_unique<Model>("sphere");
	modelSmoothSphere = std::make_unique<Model>("sphere", false, true);
#pragma endregion

	skydome.reset(Object3D::Create(modelSkydome.get()));
	ground.reset(TouchableObject::Create(modelGround.get()));
	sphere.reset(Object3D::Create(modelSphere.get()));
	sphere2.reset(Object3D::Create(modelSmoothSphere.get()));
#pragma region Texture
	reimuG = dx->LoadTextureGraph(L"Resources/reimu.png");
	grassG = dx->LoadTextureGraph(L"Resources/grass.png");
#pragma endregion

#pragma region Sprite
	sprite = std::make_unique<Sprite>(reimuG);
	sprite->SetSize(Vector2D(200.0f, 200.0f));
	//sprite->SetTextureLeftTop(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
	//sprite->SetTextureSize(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
#pragma endregion
}

void GameScene::Update()
{
#pragma region XVˆ—
	timer++;
	if (timer > 120) timer = 0;

	//	ƒJƒƒ‰Ø‚è‘Ö‚¦
	if (input->GetTrigger(DIK_V)) {
		isDebug = !isDebug;
		if (isDebug) {
			delete camera;
			camera = new MyDebugCamera();
			camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
			Player::SetCamera(camera);
			Particle::SetCamera(camera);
			Object3D::SetCamera(camera);
		}
		else {
			delete camera;
			camera = new NormalCamera();
			camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
			Player::SetCamera(camera);
			Particle::SetCamera(camera);
			Object3D::SetCamera(camera);
		}
	}
	float movelen = 3.0f;
	Vector3D spherePos = sphere->GetPosition();
	spherePos.y = -movelen + timer / 60.0f * movelen;
	sphere->SetPosition(spherePos);
	
	spherePos = sphere2->GetPosition();
	spherePos.y = movelen - timer / 60.0f * movelen;
	sphere2->SetPosition(spherePos);

	square->SetIsBillboardY(true);

	camera->Update();

	player->Update();
	ground->ColliderUpdate();

	float left = (float)input->GetKey(DIK_RIGHT) - input->GetKey(DIK_LEFT);
	float front = (float)input->GetKey(DIK_N) - input->GetKey(DIK_M);
	float up = (float)input->GetKey(DIK_DOWN) - input->GetKey(DIK_UP);
	if (left == 0 && front == 0 && up == 0) left = 1.0f;
	Light::GetInstance()->SetDirLightDir(0, { left, up, front });
	sphere->mat.rotAngle.y += 0.02f;
	sphere->ColliderUpdate();
	sphere2->mat.rotAngle.y += 0.02f;
	sphere2->ColliderUpdate();

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
	sphere2->Draw();
	player->Draw();
	square->Draw(grassG);

	sprite->Draw();
}
