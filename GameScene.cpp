#include "GameScene.h"

void GameScene::MatUpdate()
{
	ground->MatUpdate(camera->GetMatrix(), matProjection, camera->GetEye());
	skydome->MatUpdate(camera->GetMatrix(), matProjection, camera->GetEye());
	sphere->MatUpdate(camera->GetMatrix(), matProjection, camera->GetEye());
	sword->MatUpdate(camera->GetMatrix(), matProjection, camera->GetEye());
	player->MatUpdate(camera->GetMatrix(), matProjection, camera->GetEye());

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
	camera->Initialize(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	objShader.Initialize(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");

	modelpipeline = std::make_unique<GPipeline>(dx->GetDev(), objShader);

	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(45.0f), 0.1f, 1000.0f);

	Model::SetPipeline(modelpipeline.get());
	LoadResources();

	sphere->mat.trans.x = -1.0f;
	sphere->mat.trans.y = 1.0f;
	sword->mat.trans.x = 1.0f;
	sword->mat.rotAngle.y = MyMath::PI;

	player = std::make_unique<Player>();
	player->Initialize("chr_sword", true);
}

void GameScene::Finalize()
{
}

void GameScene::LoadResources()
{
#pragma region Model
	skydome = std::make_unique<Model>("skydome");
	ground = std::make_unique<Model>("ground");
	sphere = std::make_unique<Model>("sphere");
	sword = std::make_unique<Model>("chr_sword");
#pragma endregion

#pragma region AssimpModel
	fbxModel = std::make_unique<AssimpModel>(modelpipeline.get());
	fbxModel->Initialize(L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX");
#pragma endregion


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

	sphere->mat.rotAngle.y += 0.01f;
#pragma endregion
	MatUpdate();
}

void GameScene::Draw()
{
	ground->Draw();
	skydome->Draw();

	sphere->Draw();
	sword->Draw();

	player->Draw();

	sprite->Draw(reimuG);
}
