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
#include "TriangleCollider.h"

void GameScene::CollisionUpdate()
{
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(sphere->GetCollider());
	//if (collisionMan->CheckCollision(*sphereCollider, COLLISION_ATTR_PLANE)) {
	//	sphere->SetColor({ 1.0f,0.2f,0.2f });
	//}
	//else {
	//	sphere->SetColor({ 1.0f,1.0f,1.0f });
	//}
	SphereCollider* sphere2Collider = dynamic_cast<SphereCollider*>(sphere2->GetCollider());
	if (collisionMan->CheckCollision(*sphere2Collider, COLLISION_ATTR_PLANE)) {
		sphere2->SetColor({ 1.0f,0.2f,0.2f });
	}
	else {
		sphere2->SetColor({ 1.0f,1.0f,1.0f });
	}
	if (collisionMan->CheckCollision(*sphereCollider, COLLISION_ATTR_TRIANGLE)) {
		sphere->SetColor({ 1.0f,0.2f,0.2f });
	}
	else {
		sphere->SetColor({ 1.0f,1.0f,1.0f });
	}
	
	player->CollisionUpdate();
	collisionMan->CheckAllCollisions();
}

void GameScene::MatUpdate()
{
	ground2->MatUpdate();
	ground->MatUpdate();
	skydome->MatUpdate();
	player->MatUpdate();
	sphere->MatUpdate();
	sphere2->MatUpdate();
	triangle->MatUpdate();
	box->MatUpdate();

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
	ground->SetAttribute(COLLISION_ATTR_PLANE);
	ground->SetScale({ 0.5f,0.5f,0.5f });
	ground2->SetRotation({ 0.0f,0.0f,MyMath::ConvertToRad(20.0f) });
	ground2->SetPosition({ 5.0f, 0.0f, 0.0f });
	TriangleCollider* triangleColl = new TriangleCollider({ 1,0,1 }, { 1,0,-1 }, { -1,0,1 });
	triangleColl->SetObject3D(triangle.get());
	triangleColl->SetAttribute(COLLISION_ATTR_TRIANGLE);
	triangle->SetCollider(triangleColl);
	//triangle->SetPosition(Vector3D(3.0f, 2.0f, 0.0f));
	//triangle->SetRotation(Vector3D(MyMath::PI/2.0f, MyMath::ConvertToRad(60.0f) , 0.0f));

	sphere->SetCollider(new SphereCollider());
	sphere->SetPosition(Vector3D(0.0f, 4.0f, 0.0f));
	sphere2->SetCollider(new SphereCollider());
	sphere2->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
	player->PlayerInitialize(modelSword.get());

	Particle::SetCamera(camera);
	square = new Particle();

	mord = CircleCollisionPlane;

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
	modelTriangle = std::make_unique<Model>("triangle");
	modelBox = std::make_unique<Model>("box");
	modelSmoothSphere = std::make_unique<Model>("sphere", false, true);
#pragma endregion

	skydome.reset(Object3D::Create(modelSkydome.get()));
	ground.reset(TouchableObject::Create(modelGround.get()));
	ground2.reset(TouchableObject::Create(modelGround.get()));
	sphere.reset(Object3D::Create(modelSphere.get()));
	sphere2.reset(Object3D::Create(modelSmoothSphere.get()));
	triangle.reset(Object3D::Create(modelTriangle.get()));
	box.reset(Object3D::Create(modelBox.get()));
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
	if (timer > 240) timer = 0;

	ground->SetColor({ 1.0f,1.0f,1.0f });

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



	if (input->GetTrigger(DIK_1)) {
		mord = CircleCollisionPlane;
		sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
	}
	if (input->GetTrigger(DIK_2)) {
		mord = CircleCollisionTriangle;
		sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
	}
	if (input->GetTrigger(DIK_3)) {
		sphere->SetPosition(Vector3D(4.0f, 4.0f, 0.0f));
		sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
		mord = RayCollisionPlane;
	}
	if (input->GetTrigger(DIK_4)) {
		sphere->SetPosition(Vector3D(4.0f, 4.0f, 0.0f));
		sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
		mord = RayCollisionTriangle;
	}
	if (input->GetTrigger(DIK_5)) {
		sphere->SetPosition(Vector3D(0.0f, 4.0f, 0.0f));
		sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
		mord = RayCollisionSphere;
	}
	if (input->GetTrigger(DIK_6)) {
		mord = PlayerOnGround;
		sphere->SetPosition(Vector3D(0.0f, 100.0f, 0.0f));
		sphere2->SetPosition(Vector3D(0.0f, 100.0f, 0.0f));
	}

	Ray ray;
	ray.start = layStart;
	ray.dir = { 0,-1.0,0 };
	RayCast raycastHit;
	Vector3D layEnd;
	Vector3D spherePos;
	float movelen = 3.0f;
	switch (mord)
	{
	case CircleCollisionPlane:
		spherePos = sphere2->GetPosition();
		if (timer <= 120.0f) {
			spherePos.y = movelen - timer / 60.0f * movelen;
		}
		else {
			spherePos.y = -movelen + (timer - 120.0f) / 60.0f * movelen;
		}
		sphere2->SetPosition(spherePos);
		break;
	case CircleCollisionTriangle:
		break;
	case RayCollisionPlane:
		if (timer <= 120.0f) {
			layStart.y = 2.0f - 10.0f + timer / 60.0f * 10.0f;
		}
		else {
			layStart.y = 2.0f + 10.0f - (timer - 120.0f) / 60.0f * 10.0f;
		}

		//	“–‚½‚è”»’è
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, 100.0f)) {
			layEnd = layStart;
			layEnd += ray.dir * (raycastHit.distance / 2.0f);
			box->SetPosition(layEnd);
			raycastHit.object->SetColor({ 1.0f,0.2f,0.2f });
			layEnd = layStart;
			layEnd += ray.dir * raycastHit.distance;
			box->SetScale({ 0.2f,raycastHit.distance / 2.0f,0.2f });
		}
		else {
			layEnd = layStart;
			layEnd += ray.dir * 50.0f;
			box->SetPosition(layEnd);
			layEnd += ray.dir * 50.0f;
			box->SetScale({ 0.2f,50.0f,0.2f });
		}
		break;
	case RayCollisionTriangle:
		break;
	case RayCollisionSphere:
		if (timer <= 120.0f) {
			layStart.y = 2.0f - 10.0f + timer / 60.0f * 10.0f;
		}
		else {
			layStart.y = 2.0f + 10.0f - (timer - 120.0f) / 60.0f * 10.0f;
		}

		//	“–‚½‚è”»’è
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, 100.0f)) {
			layEnd = layStart;
			layEnd += ray.dir * (raycastHit.distance / 2.0f);
			box->SetPosition(layEnd);
			raycastHit.object->SetColor({ 1.0f,0.2f,0.2f });
			layEnd = layStart;
			layEnd += ray.dir * raycastHit.distance;
			box->SetScale({ 0.2f,raycastHit.distance / 2.0f,0.2f });
		}
		else {
			layEnd = layStart;
			layEnd += ray.dir * 50.0f;
			box->SetPosition(layEnd);
			layEnd += ray.dir * 50.0f;
			box->SetScale({ 0.2f,50.0f,0.2f });
		}
		break;
	case PlayerOnGround:
		break;
	default:
		break;
	}
	//Vector3D spherePos = sphere->GetPosition();
	//spherePos.y = 2.0f - movelen + timer / 60.0f * movelen;
	//sphere->SetPosition(spherePos);
	
	

	square->SetIsBillboardY(true);
	camera->Update();
	player->Update();
	ground->ColliderUpdate();
	ground2->ColliderUpdate();

	Light::GetInstance()->SetDirLightDir(0, { 0.0f, 1.0f, 0.0f});
	sphere->mat.rotAngle.y += 0.02f;
	sphere->ColliderUpdate();
	sphere2->mat.rotAngle.y += 0.02f;
	sphere2->ColliderUpdate();
	triangle->ColliderUpdate();
	box->ColliderUpdate();

	sprite->Update();
#pragma endregion
	MatUpdate();

	CollisionUpdate();
}

void GameScene::Draw()
{
	ground2->Draw();
	ground->Draw();

	switch (mord)
	{
	case CircleCollisionPlane:
		sphere->Draw();
		sphere2->Draw();
		break;
	case CircleCollisionTriangle:
		sphere->Draw();
		sphere2->Draw();
		break;
	case RayCollisionPlane:
		box->Draw();
		break;
	case RayCollisionTriangle:
		box->Draw();
		break;
	case RayCollisionSphere:
		box->Draw();
		sphere->Draw();
		sphere2->Draw();
		break;
	case PlayerOnGround:
		break;
	default:
		break;
	}

	triangle->Draw();
	player->Draw();
}
