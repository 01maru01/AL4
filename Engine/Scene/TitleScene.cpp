#include "TitleScene.h"
#include "Input.h"
#include "InputJoypad.h"
#include "SceneManager.h"
#include "NormalCamera.h"

MyXAudio* TitleScene::xAudio = nullptr;

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	xAudio = MyXAudio::GetInstance();

	camera = std::make_unique<NormalCamera>();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	LoadResources();
#pragma region Sprite
	pressSprite = std::make_unique<Sprite>(pressG);
	pressSprite->SetSize(Vector2D{ 300,32 });
	pressSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,620 });
	pressSprite->SetAnchorPoint(Vector2D{ 0.5,0.5 });
	pressSprite->TransferVertex();
#pragma endregion

	xAudio->SoundPlayLoopWave(bgmSound, 0.1f);
}

void TitleScene::Finalize()
{
	xAudio->StopAllLoopSound();
}

void TitleScene::LoadResources()
{
#pragma region Sound
	bgmSound = xAudio->SoundLoadWave("bgm.wav");
	selectSound = xAudio->SoundLoadWave("select.wav");
#pragma endregion

#pragma region Texture
	pressG = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/press.png");
#pragma endregion
}

void TitleScene::Update()
{
	timer++;
	int wavespd = 40;
	pressSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,620 + cosf(timer / (float)wavespd) * 10 });
	pressSprite->TransferVertex();
	

	if (Input::GetInstance()->GetTrigger(DIK_SPACE) || InputJoypad::GetInstance()->GetTriggerButton(XINPUT_GAMEPAD_A)) {
		xAudio->SoundPlayWave(selectSound, 0.1f);
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}

	MatUpdate();
}

void TitleScene::Draw()
{
	pressSprite->Draw();
}

void TitleScene::MatUpdate()
{
	pressSprite->MatUpdate();
}
