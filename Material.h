#pragma once
#include <string>
#include "Vector3D.h"

struct Material
{
	std::string name;
	Vector3D ambient = { 0.3f, 0.3f, 0.3f };
	Vector3D diffuse;
	Vector3D specular;
	float alpha = 1.0f;
	std::string textureFilename;
};

