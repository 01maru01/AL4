#include "GameScene.h"

void GameScene::MatUpdate()
{
	ground->MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);
	skydome->MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);
	sphere->MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);
	sword->MatUpdate(debugcamera.mat, matProjection, debugcamera.eye);

	sprite->MatUpdate();
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	debugcamera.Initialize(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	objShader.Initialize(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");

	modelpipeline = std::make_unique<GPipeline>(dx->GetDev(), objShader);

	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(45.0f), 0.1f, 1000.0f);

	LoadResources();

	sphere->mat.trans.x = -1.0f;
	sphere->mat.trans.y = 1.0f;
	sword->mat.trans.x = 1.0f;
	sword->mat.rotAngle.y = MyMath::PI;
}

void GameScene::LoadResources()
{
#pragma region Model
	skydome = std::make_unique<Model>("skydome", modelpipeline.get());
	ground = std::make_unique<Model>("ground", modelpipeline.get());
	sphere = std::make_unique<Model>("sphere", modelpipeline.get());
	sword = std::make_unique<Model>("chr_sword", modelpipeline.get());
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
	debugcamera.Update();

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

	sprite->Draw(reimuG);
}
