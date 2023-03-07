#pragma once
#include "VertIdxBuff.h"
#include "GPipeline.h"
#include "ICamera.h"

class Object2D:public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static GPipeline* pipeline;
	static MyDirectX* dx;
	static ICamera* camera;

	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;

	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;

	Object2D* parent = nullptr;

	std::vector<ScreenVertex> vertices;
	std::vector<unsigned short> indices;

	MyMath::ObjMatrix mat;
	Vector3D color = { 1.0f,1.0f,1.0f };
private:
	void SetVertices() override;
public:
	Object2D() = default;
	virtual ~Object2D() {};

	static Object2D* Create();

	static void SetPipeline(GPipeline* pipeline_);
	static void SetCamera(ICamera* camera_);

	void SetParent(Object2D* parentObj) { parent = parentObj; }

	void SetColor(const Vector3D& color_) { color = color_; }
	const Vector3D& GetColor() { return color; }

	void SetPosition(const Vector3D& pos_) { mat.trans = pos_; }
	const Vector3D& GetPosition() { return mat.trans; }

	void SetScale(const Vector3D& scale_) { mat.scale = scale_; }
	const Vector3D& GetScale() { return mat.scale; }

	void SetRotation(const Vector3D& rot_) { mat.rotAngle = rot_; }
	const Vector3D& GetRotation() { return mat.rotAngle; }

	void ObjInitialize(const Vector2D& anchorPoint);
	void MatUpdate();
	void Draw(int handle);

	MyMath::ObjMatrix& GetMatObj() { return mat; }
	const Matrix& GetMatWorld() { return mat.matWorld; }
};

