#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "ICamera.h"
#include "ParticleCommon.h"
#include "Wind.h"

class Particle :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
	static ICamera* camera;
	static ParticleCommon* common;

	struct ConstBufferDataTransform {
		Matrix mat;
		Matrix matBillboard;
		float scale;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;

	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;

	struct ConstBufferDataWind {
		Vector3D windDir;
		float pad1;
		Vector3D windForce;
		float elapsedTime;
	};
	ComPtr<ID3D12Resource> windRes;
	ConstBufferDataWind* constMapWind = nullptr;

	Vector3D vertex;
	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };

	float scale = 1.0f;

	Wind wind;

	bool isBillboard = false;
	bool isBillboardY = true;
public:
	static void SetCamera(ICamera* camera_);

	void Initialize();
	Particle();
	Particle(const Vector3D& pos_);
	void MatUpdate();
	void Draw(int handle);

	void SetColor(const Vector4D& color_) { color = color_; }
	const Vector4D& SetColor() { return color; }
	void SetScale(float scale_);
	void SetPosition(const Vector3D& pos);
	const Vector3D& GetPosition() { return vertex; }
	void Move(const Vector3D& spd);

	bool IsBillboard() { return isBillboard; }
	void SetIsBillboard(bool flag) { isBillboard = flag; }
	bool IsBillboardY() { return isBillboardY; }
	void SetIsBillboardY(bool flag) { isBillboardY = flag; }
private:
	void SetVertices() override;
};

