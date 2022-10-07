#pragma once
namespace Easing {
	double lerp(const double a, const double b, const double t);

	double EaseInOut(const double startpos, const double endpos, float time, int num);
	double EaseInBack(const double startpos, const double endpos, float time, int num = 1, double p1 = 1.70158);
	double EaseOutBack(const double startpos, const double endpos, float time, int num = 1, double p1 = 1.70158);
	double EaseIn(const double start, const double end, const double time, int num);
	double EaseOut(const double start, const double end, const double time, int num);
}

