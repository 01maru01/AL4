#pragma once

#pragma region ëOíuêÈåæ
class Matrix;
class Vector3D;
#pragma endregion

class Vector4D
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(const Vector3D& v3d, float w);

	float length() const;
	Vector4D& normalize();

#pragma region Operator
	Vector4D operator+() const;
	Vector4D operator-() const;
	Vector4D& operator-=(const Vector4D& v);
	Vector4D& operator/=(float s);
	Vector4D& operator*=(const Matrix& m);
#pragma endregion
};

#pragma region Operator
const Vector4D operator-(const Vector4D& v1, const Vector4D& v2);
#pragma endregion
