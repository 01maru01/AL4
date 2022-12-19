#pragma once
#include "SpriteCommon.h"

class Sprite :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	SpriteCommon* common = nullptr;

	Matrix matWorld;
	Matrix matRot;
	Matrix matTrans;

	float rotAngle;
	Vector2D trans;
	Vector4D color;
	Vector2D size = { 100.0f,100.0f };
	Vector2D anchorPoint;

	Vector2D textureLeftTop;
	Vector2D textureSize;

	bool isFlipX = false;
	bool isFlipY = false;
	bool isInvisible = false;

	ScreenVertex vertices[4];
	UINT vertexSize;

	enum VertexNumber {
		LB,
		LT,
		RB,
		RT,
	};
	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};

	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;
public:
	Sprite();
	void Initialize();
	void MatUpdate();
	void Draw(int handle);

	void SetPosition(const Vector2D& position) { trans = position; }
	const Vector2D& GetPosition() const { return trans; }
	void SetRotation(float rotation) { rotAngle = rotation; }
	float GetRotation() const { return rotAngle; }
	void SetColor(const Vector4D& color) { this->color = color; }
	const Vector4D& GetColor() const { return color; }

private:
	void SetVertices() override;
	void SetMatRotation();
	void SetMatTransform();
};

