#pragma once
#include "GPipeline.h"
#include "Shader.h"
#include "Matrix.h"

class ParticleCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	GPipeline pipeline;
	Shader shader;

	ParticleCommon() {};
	~ParticleCommon() {};
public:
	static ParticleCommon* GetInstance();
	static void DeleteInstance();

	ParticleCommon(const ParticleCommon& obj) = delete;
	ParticleCommon& operator=(const ParticleCommon& obj) = delete;

	void Initialize();
	void Draw();
};

