#pragma once
#include "SpriteCommon.h"
#include "VertIdxBuff.h"

class Sprite :public VertIdxBuff
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
	Vector2D textureSize = { 100.0f,100.0f };

	int handle = -1;

	bool isFlipX = false;
	bool isFlipY = false;
	bool isInvisible = false;

	std::vector<ScreenVertex> vertices;

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
	Sprite(uint32_t handle_ = UINT32_MAX);
	void Initialize(uint32_t handle_ = UINT32_MAX);
	void Update();
	void MatUpdate();
	void Draw();

	void SetPosition(const Vector2D& position) { trans = position; }
	void SetSize(const Vector2D& size_) { size = size_; }
	void SetAnchorPoint(const Vector2D& anchor) { anchorPoint = anchor; }
	void SetTextureLeftTop(const Vector2D& leftTop) { textureLeftTop = leftTop; }
	const Vector2D& GetTextureLeftTop() const { return textureLeftTop; }
	void SetTextureSize(const Vector2D& size_) { textureSize = size_; }
	const Vector2D& GetTextureSize() const { return textureSize; }
	const Vector2D& GetPosition() const { return trans; }
	const Vector2D& GetSize() const { return size; }
	void SetRotation(float rotation) { rotAngle = rotation; }
	float GetRotation() const { return rotAngle; }
	void SetColor(const Vector4D& color_) { color = color_; }
	const Vector4D& GetColor() const { return color; }
	void SetHandle(int handle_) { handle = handle_; }
	void TransferVertex();
private:
	void SetVertices() override;
	void SetMatRotation();
	void SetMatTransform();
	void AdjustTextureSize();
};

