#pragma once
#include "Matrix.h"
#include "Vector3D.h"

class Quaternion
{
public:
	float x = 0.0f;	//	ãïêî
	float y = 0.0f;	//	ãïêî
	float z = 0.0f;	//	ãïêî
	float w = 0.0f;	//	é¿êî
public:
	Quaternion() {};
	Quaternion(float w, float x, float y, float z) :w(w), x(x), y(y), z(z) {};
	Quaternion(float w, const Vector3D& vec) :w(w), x(vec.x), y(vec.y), z(vec.z) {};
	Quaternion SetQuaternion(Vector3D v);

	Vector3D GetVec() { return Vector3D(x, y, z); }
	void SetVec(const Vector3D& vec);

	Quaternion CalcProduct(const Quaternion& q, const Quaternion& r);
	Quaternion GetIdentity();
	Quaternion GetConjugation();
	void Normalize();

	Matrix GetRotMatrix();


	Quaternion& operator/=(float s);
	Quaternion& operator*=(float s);
	Quaternion& operator+=(Quaternion q);
	Quaternion& operator-=(Quaternion q);

	float Dot(const Quaternion& q);
};

Quaternion SetQuaternion(const Vector3D& v, float angle);

Quaternion operator-(const Quaternion& q);

const Quaternion operator*(const Quaternion& q, float s);
const Quaternion operator*(float s, const Quaternion& q);
Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator-(const Quaternion& q1, const Quaternion& q2);

Quaternion Multiply(const Quaternion& q, const Quaternion& r);
Quaternion IdentityQuaternion();
Quaternion Conjugate(const Quaternion& quaternion);
float Norm(const Quaternion& quaternion);
Quaternion Normalize(const Quaternion& quaternion);
Quaternion Inverse(const Quaternion& quaternion);
Quaternion MakeAxisAngle(const Vector3D& axis, float angle);
Vector3D RotateVector(const Vector3D& vec, const Quaternion& quaternion);
Matrix MakeRotateMatrix(const Quaternion& quaternion);

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
Quaternion CameraSlerp(const Quaternion& q0, const Quaternion& q1, float t);

Quaternion DirectionToDirection(const Vector3D& u, const Vector3D& v);