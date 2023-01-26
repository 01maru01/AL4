#include "Easing.h"
#include <math.h>

template<typename T>
T Easing::lerp(T a, T b, T t)
{
	return a + t * (b - a);
}
template float Easing::lerp<float>(float, float, float);
template double Easing::lerp<double>(double, double, double);

template<typename T>
T Easing::EaseInOut(T startpos, T endpos, T time, int num)
{
	if (time < 0.5)
	{
		return lerp(startpos, endpos, (pow(2, num - 1) * pow(time, num)));
	}
	return lerp(startpos, endpos, (1 - pow(-2 * time + 2, num) / 2));
}

template<typename T>
T Easing::EaseInBack(T startpos, T endpos, T time, int num, double p1)
{
	const double c1 = p1;
	const double c3 = c1 + 1;

	return lerp(startpos, endpos, pow((c3 * time * time * time - c1 * time * time), num));
}

template<typename T>
T Easing::EaseOutBack(const T startpos, const T endpos, T time, int num, double p1)
{
	if (time < 0.5)
	{
		return lerp(startpos, endpos, (pow(2, num - 1) * pow(time, num)));
	}
	return lerp(startpos, endpos, (1 - pow(-2 * time + 2, num) / 2));
}

template<typename T>
T Easing::EaseIn(T start, T end, T time, int num)
{
	return lerp(start, end, pow(time, num));
}

template<typename T>
T Easing::EaseOut(T start, T end, T time, int num)
{
	return lerp(start, end, 1 - pow(1 - time, num));
}
