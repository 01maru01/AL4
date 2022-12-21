#include "ICamera.h"

void ICamera::MatUpdate()
{
	mat = MyMath::LookAtLH(eye, target, up);
}
