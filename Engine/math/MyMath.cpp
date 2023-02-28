#include "MyMath.h"
#include <cassert>
#include <random>

Matrix MyMath::LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	Vector3D baseX = up;
	Vector3D baseY;
	Vector3D baseZ = target - eye;

	baseZ.normalize();

	baseX = baseX.cross(baseZ);
	baseX.normalize();

	baseY = baseZ;
	baseY = baseY.cross(baseX);

	Matrix matView;

	matView.m[0][0] = baseX.x;
	matView.m[1][0] = baseX.y;
	matView.m[2][0] = baseX.z;

	matView.m[0][1] = baseY.x;
	matView.m[1][1] = baseY.y;
	matView.m[2][1] = baseY.z;

	matView.m[0][2] = baseZ.x;
	matView.m[1][2] = baseZ.y;
	matView.m[2][2] = baseZ.z;

	matView.m[3][0] = -baseX.dot(eye);
	matView.m[3][1] = -baseY.dot(eye);
	matView.m[3][2] = -baseZ.dot(eye);

	return matView;
}

float MyMath::ConvertToRad(float angle)
{
	return angle / 180.0f * PI;
}

float MyMath::GetRand(float min, float max)
{
	std::random_device rd;
	std::mt19937_64 eng(rd());
	std::uniform_real_distribution<float> distr(min, max);
	return distr(eng);
}

bool MyMath::CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad)
{
	Vector3D start_to_center = Vector3D(pos.x - startL.x, pos.y - startL.y, pos.z - startL.z);
	Vector3D end_to_center = Vector3D(pos.x - endL.x, pos.y - endL.y, pos.z - endL.z);
	Vector3D start_to_end = Vector3D(endL.x - startL.x, endL.y - startL.y, endL.z - startL.z);
	// 単位ベクトル化する
	start_to_end.normalize();
	Vector3D dis = start_to_end.cross(start_to_center);

	float distance_projection = dis.length();

	if (fabs(distance_projection) >= rad) return false;

	// 始点 => 終点と始点 => 円の中心の内積を計算する
	float dot01 = start_to_center.dot(start_to_end);
	// 始点 => 終点と終点 => 円の中心の内積を計算する
	float dot02 = end_to_center.dot(start_to_end);

	// 二つの内積の掛け算結果が0以下なら当たり
	if (dot01 * dot02 <= 0.0f)
	{
		return true;
	}

	if (start_to_center.length() < rad ||
		end_to_center.length() < rad)
	{
		return true;
	}

	return false;
}

Matrix MyMath::PerspectiveFovLH(const int winwidth, const int winheight, float fovY, float nearZ, float farZ)
{
	assert(nearZ > 0.f && farZ > 0.f);
	//assert(!XMScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
	//assert(!XMScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
	//assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

	float aspect = (float)winwidth / winheight;

	//float tan = (float)sin(fovY/2.0f) / cos(fovY/2.0f) * aspect;
	float height = 1.0f / tanf(fovY / 2.0f);

	Matrix matProjection;
	matProjection.Identity();
	matProjection.m[0][0] = height;
	matProjection.m[1][1] = height * aspect;
	matProjection.m[2][2] = (float)(farZ + nearZ) / (farZ - nearZ);
	matProjection.m[2][3] = 1.0f;
	matProjection.m[3][2] = -2.0f * farZ * nearZ / (float)(farZ - nearZ);
	matProjection.m[3][3] = 0.0f;

	return matProjection;
}

Matrix MyMath::OrthoLH(const int winwidth, const int winheight, float nearZ, float farZ)
{
	Matrix matProjection;
	matProjection.Identity();
	matProjection.m[0][0] = 2 / (float)winwidth;
	matProjection.m[1][1] = 2 / (float)winheight;
	matProjection.m[2][2] = 1 / (float)(farZ - nearZ);
	matProjection.m[3][2] = nearZ / (float)(nearZ - farZ);

	return matProjection;
}

//MyMath::MatView::MatView(Vector3D _eye, Vector3D _target, Vector3D _up)
//{
//	Init(_eye, _target, _up);
//}

MyMath::MatView::MatView()
{
	Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
}

void MyMath::MatView::Init(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	eye = _eye;
	target = _target;
	up = _up;

	MatUpdate();
}

void MyMath::MatView::MatUpdate()
{
	mat = LookAtLH(eye, target, up);
}

MyMath::float4::float4() :
	x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

MyMath::float4::float4(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{
}

void MyMath::ObjMatrix::SetMatScaling()
{
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
}

void MyMath::ObjMatrix::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotX;
	matRotX.m[1][1] = cos(rotAngle.x);
	matRotX.m[1][2] = sin(rotAngle.x);
	matRotX.m[2][1] = -sin(rotAngle.x);
	matRotX.m[2][2] = cos(rotAngle.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotAngle.y);
	matRotY.m[2][0] = sin(rotAngle.y);
	matRotY.m[0][2] = -sin(rotAngle.y);
	matRotY.m[2][2] = cos(rotAngle.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotAngle.z);
	matRotZ.m[0][1] = sin(rotAngle.z);
	matRotZ.m[1][0] = -sin(rotAngle.z);
	matRotZ.m[1][1] = cos(rotAngle.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
}

void MyMath::ObjMatrix::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}

void MyMath::ObjMatrix::Initialize()
{
	scale = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle = Vector3D(0.0f, 0.0f, 0.0f);
	trans = Vector3D(0.0f, 0.0f, 0.0f);
}

void MyMath::ObjMatrix::Update()
{
	matWorld.Identity();

	//	スケーリング
	SetMatScaling();
	matWorld *= matScale;

	//	回転
	SetMatRotation();
	matWorld *= matRot;

	//	平行移動
	SetMatTransform();
	matWorld *= matTrans;
}
