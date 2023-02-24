#pragma once
#include "Matrix.h"

class Vector2D;

class Vector3D
{
public:
	float x;
	float y;
	float z;
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector2D& vec, float z);

	float length() const;

	Vector3D& normalize();
	float dot(const Vector3D& v) const;
	Vector3D cross(const Vector3D& v) const;

	Vector3D operator+() const;

	Vector3D operator-() const;

	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);

	Vector3D& operator/=(float s);
	Vector3D& operator*=(float s);
};

const Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
const Vector3D operator*(const Vector3D& v, float s);
const Vector3D operator*(float s, const Vector3D& v);

const Vector3D Vec3Transform(const Vector3D& v, const Matrix& m);
const Vector3D Vec3TransformNormal(const Vector3D& v, const Matrix& m);
//���_ABC�ō��ꂽ�|���S������@�����v�Z����B
Vector3D CreatePolygonNormal(Vector3D a, Vector3D b, Vector3D c);