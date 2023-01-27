#pragma once
#include "GPipeline.h"
#include "VertIdxBuff.h"

class ScreenPolygon :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;

	GPipeline pipeline;
	std::vector<ScreenVertex> vertices;
	UINT indexSize;
	unsigned short indices[6];

	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };
public:
	ScreenPolygon();
	void Draw();
	void SetColor(const Vector4D& color_);
private:
	void SetVertices() override;
};

