#pragma once
#ifndef SCALARMATH_H
#define SCALARMATH_H
#include "PCH.h"

template<typename T>
T Saturate(const T &f)
{
	return max(min(1, f), 0);
}

template<typename T>
T AngleToRadian(const T &angle)
{
	return angle * static_cast<float>(M_RAD_DEGREE);
}

template<typename T>
T RadianToAngle(const T &rad)
{
	return rad * static_cast<float>(M_DEGREE_RAD);
}
#endif // !SCALARMATH_H
