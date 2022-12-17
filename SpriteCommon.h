#pragma once
#include "GPipeline.h"
#include "VertBuff.h"
#include "Shader.h"

class SpriteCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	GPipeline pipeline;
	Shader shader;

	Matrix mat2D;
public:
	SpriteCommon();
	void Initialize();
	void Draw();

	const Matrix& Get2DMat() const { return mat2D; }
};

