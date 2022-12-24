#include "ICamera.h"

void ICamera::MatUpdate()
{
	matView = MyMath::LookAtLH(eye, target, up);
}
