#include "Framework.h"

void Framework::Run()
{
	Initialize();
	//	ƒQ[ƒ€ƒ‹[ƒv
	while (true)
	{
		Update();

		if (IsEndGameRoop()) { break; }

		Draw();
	}
	Finalize();
}

void Framework::Initialize()
{
	win = Window::GetInstance();
	win->Initialize();

	textureMan = TextureManager::GetInstance();
	textureMan->Initialize();

	dx = MyDirectX::GetInstance();
	dx->Initialize();

	textureMan->SetWhiteTexHandle();

	joypad = InputJoypad::GetInstance();
	input = Input::GetInstance();

	particleCommon = ParticleCommon::GetInstance();
	particleCommon->Initialize();
}

void Framework::Update()
{
	isEndRoopFlag = win->MsgUpdate() || input->GetTrigger(DIK_ESCAPE);

	input->Update();
	joypad->Update();
}

void Framework::Finalize()
{
	ParticleCommon::DeleteInstance();
	SpriteCommon::DeleteInstance();
	InputJoypad::DeleteInstance();
	Input::DeleteInstance();
	MyXAudio::DeleteInstance();
	MyDirectX::DeleteInstance();
	Window::DeleteInstance();
}
