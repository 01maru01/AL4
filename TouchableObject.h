#pragma once
#include "Object3d.h"

class TouchableObject : public Object3D
{
public:
	static TouchableObject* Create(Model* model = nullptr);
	bool Initialize(Model* model);
};

