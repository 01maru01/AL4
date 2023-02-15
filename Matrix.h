#pragma once

class Matrix
{
public:
	float m[4][4];
public:
	Matrix();
	void Identity();
	Matrix& operator*=(const Matrix& m2);
};

bool InverseMatrix(const Matrix& mat, Matrix& invMat);

