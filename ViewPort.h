#pragma once
#include "Matrix.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class ViewPort
{
private:
	D3D12_VIEWPORT viewport{};
	Matrix mat;
public:
	void Init(const int width, const int height, const int topLftX, const int topLftY, const float MinDepth = 0.0f, const float MaxDepth = 1.0f);
	ViewPort();
	void Update(ID3D12GraphicsCommandList* cmdList);
	Matrix Mat() { return mat; }
};

