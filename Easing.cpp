#include "Easing.h"
#include <math.h>

double Easing::lerp(const double a, const double b, const double t)
{
	return a + t * (b - a);
}

double Easing::EaseInOut(const double startpos, const double endpos, float time, int num)
{
	if (time < 0.5)
	{
		return lerp(startpos, endpos, (pow(2, num - 1) * pow(time, num)));
	}
	return lerp(startpos, endpos, (1 - pow(-2 * time + 2, num) / 2));
}

double Easing::EaseInBack(const double startpos, const double endpos, float time, int num, double p1)
{
	const double c1 = p1;
	const double c3 = c1 + 1;

	return lerp(startpos, endpos, pow((c3 * time * time * time - c1 * time * time), num));
}

double Easing::EaseOutBack(const double startpos, const double endpos, float time, int num, double p1)
{
	if (time < 0.5)
	{
		return lerp(startpos, endpos, (pow(2, num - 1) * pow(time, num)));
	}
	return lerp(startpos, endpos, (1 - pow(-2 * time + 2, num) / 2));
}

double Easing::EaseIn(const double start, const double end, const double time, int num)
{
	return lerp(start, end, pow(time, num));
}

double Easing::EaseOut(const double start, const double end, const double time, int num)
{
	return lerp(start, end, 1 - pow(1 - time, num));
}
