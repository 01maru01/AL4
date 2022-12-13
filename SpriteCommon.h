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
	void Initialize(ID3D12Device* dev);
	void Draw();

	const Matrix& Get2DMat() const { return mat2D; }
};

