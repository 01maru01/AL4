#include "VolumeLight.h"
int VolumeLightObj::handle = -1;

void VolumeLightObj::SetLightGraph(int handle_)
{
	handle = handle_;
}

void VolumeLightObj::Initialize(const Vector2D& scale_, const Vector3D& pos, const Vector3D& dir)
{
	for (int i = 0; i < 2; i++) {
		lightObj[i].ObjInitialize(Vector2D(0.5f, 0.0f));
		lightObj[i].GetMatObj().scale = Vector3D(scale_, 1.0f);
		lightObj[i].GetMatObj().trans = pos;
	}
	lightObj[1].GetMatObj().rotAngle.y = MyMath::ConvertToRad(90.0f);
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
