#pragma once

namespace Easing {
	template<typename T>
	T lerp(T a, T b, T t);

	template<typename T>
	T EaseInOut(T startpos, T endpos, T time, int num);
	template<typename T>
	T EaseInBack(T startpos, T endpos, T time, int num = 1, double p1 = 1.70158);
	template<typename T>
	T EaseOutBack(const T startpos, const T endpos, T time, int num = 1, double p1 = 1.70158);
	template<typename T>
	T EaseIn(T start, T end, T time, int num);
	template<typename T>
	T EaseOut(T start, T end, T time, int num);
}

