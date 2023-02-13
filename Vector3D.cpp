#include "Vector3D.h"

#include <cmath>

Vector3D::Vector3D()
	:x(0), y(0), z(0)
{
}

Vector3D::Vector3D(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

float Vector3D::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector3D& Vector3D::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float Vector3D::dot(const Vector3D& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::cross(const Vector3D& v) const
{
	return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3D Vector3D::operator+() const
{
	return *this;
}

Vector3D Vector3D::operator-() const
{
	return Vector3D(-x, -y, -z);
}

Vector3D& Vector3D::operator+=(const Vector3D& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3D& Vector3D::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3D& Vector3D::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

const Vector3D operator-(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D temp(v1);
	return temp -= v2;
}

const Vector3D operator*(const Vector3D& v, float s)
{
	Vector3D temp(v);
	return temp *= s;
}

const Vector3D operator*(float s, const Vector3D& v)
{
	return v * s;
}

const Vector3D MyMath::Normalize(const Vector3D& v)
{
	Vector3D vec = v;
	float len = vec.length();
	if (len != 0)
	{
		return vec /= len;
	}
	return vec;
}

float MyMath::Dot(const Vector3D& v1, const Vector3D& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

const Vector3D MyMath::Cross(const Vector3D& v1, const Vector3D& v2)
{
	return Vector3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

const Vector3D MyMath::Vec3Transform(const Vector3D& v, const Matrix& m)
{
	Vector3D ans;
	ans.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	ans.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	ans.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	//ans.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	
	return ans;
}

const Vector3D MyMath::Vec3TransformNormal(const Vector3D& v, const Matrix& m)
{
	Vector3D ans;
	ans.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	ans.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	ans.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	//ans.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	return ans;
}

const Vector3D MyMath::CreatePolygonNormal(const Vector3D& a, const Vector3D& b, const Vector3D& c)
{
	Vector3D AB(b - a);
	Vector3D BC(c - b);

	Vector3D normal = AB;
	normal.cross(BC);
	normal.normalize();

	return normal;
}
