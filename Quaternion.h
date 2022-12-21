#pragma once
#include "Matrix.h"
#include "Vector3D.h"

class Quaternion
{
private:
	float x;	//	‹•”
	float y;	//	‹•”
	float z;	//	‹•”
	float w;	//	À”
public:
	void SetQuaternion(Vector3D v, float angle);
	Quaternion SetQuaternion(Vector3D v);


	Quaternion CalcProduct(const Quaternion& q, const Quaternion& r);
	Quaternion GetIdentity();
	Quaternion GetConjugation();
	void Normalize();

	Matrix GetRotMatrix();
};

