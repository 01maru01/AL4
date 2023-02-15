#pragma once

#pragma region 前置宣言
class Matrix;
#pragma endregion


class Vector3D
{
public:
	float x;
	float y;
	float z;
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	//	長さ
	float length() const;
	//	単位化
	Vector3D& normalize();
	//	内積
	float dot(const Vector3D& v) const;
	//	外積
	Vector3D cross(const Vector3D& v) const;

#pragma region Operator
	Vector3D operator+() const;
	Vector3D operator-() const;
	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);
	Vector3D& operator/=(float s);
	Vector3D& operator*=(float s);
#pragma endregion
};

#pragma region Operator
const Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
const Vector3D operator*(const Vector3D& v, float s);
const Vector3D operator*(float s, const Vector3D& v);
#pragma endregion

namespace MyMath {
	const Vector3D Normalize(const Vector3D& v);
	float Dot(const Vector3D& v1, const Vector3D& v2);
	const Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
	const Vector3D Vec3Transform(const Vector3D& v, const Matrix& m);
	const Vector3D Vec3TransformNormal(const Vector3D& v, const Matrix& m);
	//頂点ABCで作られたポリゴンから法線を計算する。
	const Vector3D CreatePolygonNormal(const Vector3D& a, const Vector3D& b, const Vector3D& c);
}