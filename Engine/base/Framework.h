#pragma once
#include "Input.h"
#include "InputJoypad.h"
#include "DirectX.h"
#include "MyXAudio.h"
#include "SpriteCommon.h"
#include "Object3D.h"
#include "ParticleCommon.h"
#include "TextureManager.h"

class Framework
{
protected:
	Input* input = nullptr;
	InputJoypad* joypad = nullptr;
	Window* win = nullptr;
	MyDirectX* dx = nullptr;
	ParticleCommon* particleCommon = nullptr;
	TextureManager* textureMan = nullptr;

	bool isEndRoopFlag = false;
public:
	virtual ~Framework() = default;

	void Run();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw() = 0;
	virtual void Finalize();
	virtual bool IsEndGameRoop() { return isEndRoopFlag; }
};

