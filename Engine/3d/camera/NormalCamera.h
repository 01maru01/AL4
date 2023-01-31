#pragma once
#include "ICamera.h"
class NormalCamera :public ICamera
{
public:
	NormalCamera();
	~NormalCamera() override;

	void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) override;
	void Update() override;
};
