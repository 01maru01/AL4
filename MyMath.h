#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include <cmath>

namespace MyMath {
	const float PI = 3.14159265358979f;
	const float PIx2 = 6.2831853071f;
	
	//	視点座標、注視点座標、上方向ベクトル
	const Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);
	//	射影変換行列
	const Matrix PerspectiveFovLH(const int winwidth, const int winheight, float fovY, float nearZ, float farZ);
	//	平行投影行列
	const Matrix OrthoLH(const int winwidth, const int winheight, float nearZ, float farZ);
	
	float ConvertToRad(float angle);	//	ラジアン変換
	
	bool CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad);
	
	class MatView{
		public:
			Matrix mat;
			Vector3D eye;		//	視点座標
			Vector3D target;	//	注視点座標
			Vector3D up;		//	上方向ベクトル
			//	カメラの方向ベクトル
			Vector3D frontVec;
			Vector3D rightVec;
			Vector3D downVec;
			//	ビルボード
			Matrix billboard;
			Matrix billboardY;
		public:
			MatView();
			void Init(Vector3D _eye, Vector3D _target, Vector3D _up);
			void CalcCameraDirVec();	//	カメラ方向ベクトル計算
			void CalcBillboard();		//	ビルボード計算
			void SetMatrix();			//	ビュー行列計算
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

