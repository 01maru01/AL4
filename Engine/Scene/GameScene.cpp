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
	tree->MatUpdate();
	sphere2->MatUpdate();

	square->MatUpdate();
	
	test2d->MatUpdate();
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

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};
	modelpipeline = std::make_unique<GPipeline>();
	modelpipeline->Init(objShader, inputLayout, _countof(inputLayout), 4, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	modelpipeline->SetBlend(GPipeline::ALPHA_BLEND);

	Object3D::SetPipeline(modelpipeline.get());
	Object3D::SetCamera(camera);
	LoadResources();

	D3D12_INPUT_ELEMENT_DESC inputLayout2D[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};

	Shader test2dShader(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");
	obj2Dpipeline = std::make_unique<GPipeline>();
	obj2Dpipeline->Init(test2dShader, inputLayout2D, _countof(inputLayout2D), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	obj2Dpipeline->SetBlend(GPipeline::ADD_BLEND);
	Object2D::SetPipeline(obj2Dpipeline.get());
	Object2D::SetCamera(camera);

	sphere2->SetCollider(new SphereCollider());
	sphere2->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	sphere2->SetPosition(Vector3D(-3.0f, 1.0f, 0.0f));
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
	player->PlayerInitialize(modelSword.get());

	Particle::SetCamera(camera);
	square = new Particle();

	test2d->GetMatObj().scale = Vector3D(1.0f,100.0f,1.0f);

	mord = Phong;
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
	modelTree = std::make_unique<Model>("tree");
	modelSmoothSphere = std::make_unique<Model>("sphere", false, true);
#pragma endregion

	skydome.reset(Object3D::Create(modelSkydome.get()));
	ground.reset(TouchableObject::Create(modelGround.get()));
	tree.reset(Object3D::Create(modelTree.get()));
	sphere2.reset(Object3D::Create(modelSmoothSphere.get()));
#pragma region Texture
	test2d.reset(Object2D::Create());
	reimuG = dx->LoadTextureGraph(L"Resources/reimu.png");
	grassG = dx->LoadTextureGraph(L"Resources/grass.png");
#pragma endregion

#pragma region Sprite
	sprite = std::make_unique<Sprite>(reimuG);
	sprite->SetSize(Vector2D(200.0f, 200.0f));
	//sprite->SetTextureLeftTop(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
	//sprite->SetTextureSize(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
#pragma endregion
	lightG = dx->LoadTextureGraph(L"Resources/lightTex.jpg");
}

void GameScene::Update()
{
#pragma region 更新処理

	//	カメラ切り替え
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

	if (input->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}

	square->SetIsBillboard(input->GetKey(DIK_R));
	square->SetIsBillboardY(input->GetKey(DIK_T));

	camera->Update();

	player->Update();
	ground->ColliderUpdate();

	float left = (float)input->GetKey(DIK_RIGHT) - input->GetKey(DIK_LEFT);
	float front = (float)input->GetKey(DIK_N) - input->GetKey(DIK_M);
	float up = (float)input->GetKey(DIK_DOWN) - input->GetKey(DIK_UP);
	if (left == 0 && front == 0 && up == 0) left = 1.0f;
	//Light::GetInstance()->SetDirLightDir(0, { left, up, front });
	//sphere->mat.rotAngle.y += 0.02f;
	tree->ColliderUpdate();
	sphere2->mat.rotAngle.y += 0.02f;
	sphere2->ColliderUpdate();

	sprite->Update();
#pragma endregion
	MatUpdate();

	CollisionUpdate();

	if (input->GetTrigger(DIK_1)) {
		mord = Phong;

		Light::GetInstance()->SetCircleShadowActive(0, false);
		Light::GetInstance()->SetSpotLightActive(0, false);
		Light::GetInstance()->SetDirLightActive(0, true);
		Light::GetInstance()->SetPointLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(1, false);
		Light::GetInstance()->SetPointLightActive(2, false);
	}
	else if (input->GetTrigger(DIK_2)) {
		mord = PointLight;

		Light::GetInstance()->SetCircleShadowActive(0, false);
		Light::GetInstance()->SetSpotLightActive(0, false);
		Light::GetInstance()->SetDirLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(0, true);
		Light::GetInstance()->SetPointLightActive(1, true);
		Light::GetInstance()->SetPointLightActive(2, true);
	}
	else if (input->GetTrigger(DIK_3)) {
		mord = SpotLight;

		Light::GetInstance()->SetCircleShadowActive(0, false);
		Light::GetInstance()->SetSpotLightActive(0, true);
		Light::GetInstance()->SetDirLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(1, false);
		Light::GetInstance()->SetPointLightActive(2, false);
	}
	else if (input->GetTrigger(DIK_4)) {
		mord = CircleShadow;

		Light::GetInstance()->SetSpotLightActive(0, false);
		Light::GetInstance()->SetCircleShadowActive(0, true);
		Light::GetInstance()->SetDirLightActive(0, true);
		Light::GetInstance()->SetPointLightActive(0, false);
		Light::GetInstance()->SetPointLightActive(1, false);
		Light::GetInstance()->SetPointLightActive(2, false);
	}

	Vector3D playerGroundPos = player->GetPosition();
	playerGroundPos.y += 0.1f;
	Light::GetInstance()->SetCircleShadowCasterPos(0, playerGroundPos);
	Light::GetInstance()->SetCircleShadowDir(0, { 0.0f,1.0f,0.0f });
	Light::GetInstance()->SetCircleShadowFactorAngle(0, { 0.0f,0.5f });
	Light::GetInstance()->SetCircleShadowAtten(0, { 0.2f,0.2f,0.0f });
}

void GameScene::Draw()
{
	ground->Draw();
	skydome->Draw();
	tree->Draw();
	//sphere2->Draw();
	//player->Draw();
	//square->Draw(grassG);

	test2d->Draw(lightG);
	//sprite->Draw();
}
