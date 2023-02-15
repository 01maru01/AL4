#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include <cmath>

namespace MyMath {
	const float PI = 3.14159265358979f;
	const float PIx2 = 6.2831853071f;
	
	//	���_���W�A�����_���W�A������x�N�g��
	const Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);
	//	�ˉe�ϊ��s��
	const Matrix PerspectiveFovLH(const int winwidth, const int winheight, float fovY, float nearZ, float farZ);
	//	���s���e�s��
	const Matrix OrthoLH(const int winwidth, const int winheight, float nearZ, float farZ);
	
	float ConvertToRad(float angle);	//	���W�A���ϊ�
	
	bool CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad);
	
	class MatView{
		public:
			Matrix mat;
			Vector3D eye;		//	���_���W
			Vector3D target;	//	�����_���W
			Vector3D up;		//	������x�N�g��
			//	�J�����̕����x�N�g��
			Vector3D frontVec;
			Vector3D rightVec;
			Vector3D downVec;
			//	�r���{�[�h
			Matrix billboard;
			Matrix billboardY;
		public:
			MatView();
			void Init(Vector3D _eye, Vector3D _target, Vector3D _up);
			void CalcCameraDirVec();	//	�J���������x�N�g���v�Z
			void CalcBillboard();		//	�r���{�[�h�v�Z
			void SetMatrix();			//	�r���[�s��v�Z
	};
	
	class ObjMatrix {
		public:
			Matrix matWorld;
	
			Matrix matScale;
			Vector3D scale;
	
			Matrix matRot;
			Vector3D rotAngle;
	
			Matrix matTrans;
			Vector3D trans;
	
		private:
			void SetMatScaling();
			void SetMatRotation();
			void SetMatTransform();
		public:
			void Initialize();
			void Update();
	};
}

