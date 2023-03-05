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
	hill->MatUpdate();

	skydome->MatUpdate();
	player->MatUpdate();
	tree->MatUpdate();
	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->MatUpdate();
	}
	sphere2->MatUpdate();

	for (int i = 0; i < grass.size(); i++)
	{
		grass[i].MatUpdate();
	}

	for (int i = 0; i < testVolLight.size(); i++)
	{
		testVolLight[i].MatUpdate();
	}
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;
	for (int i = 0; i < tree2.size(); i++)
	{
		delete tree2[i];
	}
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
	obj2Dpipeline->Init(test2dShader, inputLayout2D, _countof(inputLayout2D), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, D3D12_DEPTH_WRITE_MASK_ZERO);
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
	grass.push_back(Particle());
	float dis = 6.0f;
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f,dis), 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f,dis), 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f,dis), 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f), 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis), 0.0f, -3.0f)));

	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -3.0f)));

	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 0.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -3.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -3.0f)));

	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) + dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) + dis, 0.0f, -3.0f-2.0f)));

	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 0.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, 2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -2.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-dis, 0.0f) - dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -3.0f-2.0f)));
	grass.push_back(Particle(Vector3D(MyMath::GetRand(-0.0f, dis) - dis, 0.0f, -3.0f-2.0f)));

	//testVolLight.push_back(VolumeLightObj(Vector2D(5.0f, 25.0f), Vector3D(0.0f, 0.0f, -2.0f), 30.0f));
	testVolLight.push_back(VolumeLightObj(Vector2D(5.0f, 40.0f), Vector3D(5.0f, -15.0f, 2.0f), 30.0f));
	testVolLight.push_back(VolumeLightObj(Vector2D(5.0f, 35.0f), Vector3D(8.0f, -10.0f, -2.0f), 30.0f));
	testVolLight.push_back(VolumeLightObj(Vector2D(5.0f, 25.0f), Vector3D(10.0f, 0.0f, 2.0f), 30.0f));

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
	//	丘
	modelHill = std::make_unique<Model>("ground1");
	modelTree = std::make_unique<Model>("tree");
	modelTree2 = std::make_unique<Model>("tree2");
	modelSmoothSphere = std::make_unique<Model>("sphere", false, true);
#pragma endregion
	//	天球
	skydome.reset(Object3D::Create(modelSkydome.get()));
	//	地面
	ground.reset(TouchableObject::Create(modelGround.get()));
	ground->SetScale(Vector3D(1.0f, 1.0f, 0.1f));
	hill.reset(Object3D::Create(modelHill.get()));
	float groundscale = 10.0f;
	hill->SetScale(Vector3D(groundscale, 1.0f, 1.0f));
	hill->SetPosition(Vector3D(0.0f, 0.0f, 10.0f));

	tree.reset(Object3D::Create(modelTree.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->SetPosition(Vector3D(-12.0f + 2.0f * i, 0.0f, 5.0f+MyMath::GetRand(0.0f,6.0f)));
	}
	sphere2.reset(Object3D::Create(modelSmoothSphere.get()));
#pragma region Texture
	VolumeLightObj::SetLightGraph(dx->LoadTextureGraph(L"Resources/lightTex.jpg"));
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

	//grass->SetIsBillboard(input->GetKey(DIK_R));
	//grass->SetIsBillboardY(input->GetKey(DIK_T));

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
		Light::GetInstance()->SetSpotLightActive(0, true);
		Light::GetInstance()->SetSpotLightActive(1, true);
		Light::GetInstance()->SetSpotLightActive(2, true);
		Light::GetInstance()->SetDirLightActive(0, false);
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
	//	地面
	ground->Draw();
	hill->Draw();
	//	天球
	skydome->Draw();
	//	木
	tree->Draw();
	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->Draw();
	}
	
	for (int i = 0; i < grass.size(); i++)
	{
		grass[i].Draw(grassG);
	}
	for (int i = 0; i < testVolLight.size(); i++)
	{
		testVolLight[i].Draw();
	}

	//sprite->Draw();
}
