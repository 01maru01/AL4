#pragma once
#include "MyMath.h"

struct ParticleVertex
{
	Vector3D pos;
	float scale;
};

struct Vertex
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
};

struct FBXVertex
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
	UINT boneIndex[4];
	float boneWeight[4];
};

struct VertexObj
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
	Vector4D color;
};

struct ScreenVertex
{
	Vector3D pos;
	Vector2D uv;
};