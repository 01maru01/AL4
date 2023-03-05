#include "VolumeLight.h"
int VolumeLightObj::handle = -1;

VolumeLightObj::VolumeLightObj(const Vector2D& scale_, const Vector3D& pos, float angle)
{
	Initialize(scale_, pos, angle);
}

void VolumeLightObj::SetLightGraph(int handle_)
{
	handle = handle_;
}

void VolumeLightObj::Initialize(const Vector2D& scale_, const Vector3D& pos, float angle)
{
	for (int i = 0; i < 2; i++) {
		lightObj[i].ObjInitialize(Vector2D(0.5f, 0.0f));
	}
	lightObj[0].GetMatObj().scale = Vector3D(scale_, 1.0f);
	lightObj[0].GetMatObj().trans = pos;
	lightObj[0].GetMatObj().rotAngle.z = MyMath::ConvertToRad(angle);
	//lightObj[1].SetParent(&lightObj[0]);
	//lightObj[1].GetMatObj().rotAngle.y = MyMath::ConvertToRad(90.0f);
}

void VolumeLightObj::MatUpdate()
{
	for (int i = 0; i < 2; i++) {
		lightObj[i].MatUpdate();
	}
}

void VolumeLightObj::Draw()
{
	for (int i = 0; i < 2; i++) {
		lightObj[i].Draw(handle);
	}
}
