#pragma once

class Vector2D
{
public:
	float x;
	float y;
public:
	Vector2D();
	Vector2D(float x, float y);

	float length() const;
	Vector2D& normalize();

#pragma region  Operator
	const Vector2D operator+() const;
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator/=(float s);
	Vector2D& operator*=(float s);
#pragma endregion
};

#pragma region Operator
const Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
const Vector2D operator*(const Vector2D& v, float s);
const Vector2D operator*(float s, const Vector2D& v);
#pragma endregion

namespace MyMath {
	const Vector2D Normalize(const Vector2D& v);
	float Dot(const Vector2D& v1, const Vector2D& v2);
}