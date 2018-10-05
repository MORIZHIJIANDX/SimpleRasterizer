#pragma once
#ifndef VECTORMATH_H
#define VECTORMATH_H

#include "Vector.h"
#include "Matrix.h"
#include "ScalarMath.h"

template<typename T>
Vec3<T> Transform(const Vec3<T> &v, const Matrix3x3<T> &m)
{
	Vec4<T> temp;
	temp.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	temp.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	temp.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];

	return temp;
}

//向量矩阵相乘(row-major order)
template<typename T>
Vec4<T> Transform(const Vec4<T> &v, const Matrix4x4<T> &m)
{
	Vec4<T> temp;
	temp.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
	temp.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
	temp.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
	temp.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
	
	return temp;
}

template<typename T>
Vec3<T> Vec3TransformNormal(const Vec3<T> &v, const Matrix4x4<T> &m)
{
	Vec3<T> temp;
	temp.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	temp.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	temp.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];

	return temp;
}

template<typename T>
Vec3<T> VectorReflect(const Vec3<T> &incident, const Vec3<T> &normal)
{
	Vec3<T> proj_in = normal * Dot(incident, normal) * -2.0f;

	return incident + proj_in;
}

template<typename T>
Vec4<T> VectorReflect(const Vec4<T> &incident, const Vec4<T> &normal)
{
	Vec4<T> proj_in = normal * -2 * Dot(incident, normal);

	return incident + proj_in;
}

template<typename T>
Vec2<T> VectorAbs(const Vec2<T> &v)
{
	return Vec2<T>(std::abs(v.x), std::abs(v.y));
}

template<typename T>
Vec3<T> VectorAbs(const Vec3<T> &v)
{
	return Vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template<typename T>
Vec4<T> VectorAbs(const Vec4<T> &v)
{
	return Vec4<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
}

template<typename T>
Vec2<T> VectorClamp(const Vec2<T> &v, const Vec2<T> &min, const Vec2<T> &max)
{
	return Vec2<T>(max(min(v.x, max.x), min.x),
		max(min(v.y, max.y), min.y));
}

template<typename T>
Vec3<T> VectorClamp(const Vec3<T> &v, const Vec3<T> &min, const Vec3<T> &max)
{
	return Vec3<T>(max(min(v.x, max.x), min.x),
		max(min(v.y, max.y), min.y),
		max(min(v.z, max.z), min.z));
}

template<typename T>
Vec4<T> VectorClamp(const Vec4<T> &v, const Vec4<T> &min, const Vec4<T> &max)
{
	return Vec3<T>(max(min(v.x, max.x), min.x),
		max(min(v.y, max.y), min.y),
		max(min(v.z, max.z), min.z),
		max(min(v.w, max.w), min.w));
}

template<typename T>
Vec2<T> VectorFloor(const Vec2<T> &v)
{
	return Vec2<T>(std::floor(v.x), std::floor(v.y));
}

template<typename T>
Vec3<T> VectorFloor(const Vec3<T> &v)
{
	return Vec3<T>(std::floor(v.x), std::floor(v.y), std::floor(v.z));
}

template<typename T>
Vec4<T> VectorFloor(const Vec4<T> &v)
{
	return Vec4<T>(std::floor(v.x), std::floor(v.y), std::floor(v.z), std::floor(v.w));
}

template<typename T>
Vec2<T> VectorRound(const Vec2<T> &v)
{
	return Vec2<T>(std::round(v.x), std::round(v.y), std::round(v.z));
}

template<typename T>
Vec3<T> VectorRound(const Vec3<T> &v)
{
	return Vec4<T>(std::round(v.x), std::round(v.y), std::round(v.z));
}

template<typename T>
Vec4<T> VectorRound(const Vec4<T> &v)
{
	return Vec4<T>(std::round(v.x), std::round(v.y), std::round(v.z), std::round(v.w));
}

template<typename T>
Vec2<T> VectorSaturate(const Vec2<T> &v)
{
	Vec2<T> temp;

	temp.x = Saturate<T>(v.x);
	temp.y = Saturate<T>(v.y);

	return temp;
}

template<typename T>
Vec3<T> VectorSaturate(const Vec3<T> &v)
{
	Vec3<T> temp;

	temp.x = Saturate<T>(v.x);
	temp.y = Saturate<T>(v.y);
	temp.z = Saturate<T>(v.z);

	return temp;
}

template<typename T>
Vec4<T> VectorSaturate(const Vec4<T> &v)
{
	Vec4<T> temp;

	temp.x = Saturate<T>(v.x);
	temp.y = Saturate<T>(v.y);
	temp.z = Saturate<T>(v.z);
	temp.w = Saturate<T>(v.w);

	return temp;
}

template<typename T>
Vec2<T> VectorSqrt(const Vec2<T> &v)
{
	return Vec2<T>(std::sqrt(v.x), std::sqrt(v.y));
}

template<typename T>
Vec3<T> VectorSqrt(const Vec3<T> &v)
{
	return Vec3<T>(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z));
}

template<typename T>
Vec4<T> VectorSqrt(const Vec4<T> &v)
{
	return Vec4<T>(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z), std::sqrt(v.w));
}

template<typename T>
Vec2<T> VectorPow(const Vec2<T> &v, const Vec2<T> &e)
{
	return Vec2<T>(std::pow(v.x, e.x), std::pow(v.y, e.y));
}

template<typename T>
Vec3<T> VectorPow(const Vec3<T> &v, const Vec3<T> &e)
{
	return Vec3<T>(std::pow(v.x, e.x), std::pow(v.y, e.y), std::pow(v.z, e.z));
}

template<typename T>
Vec4<T> VectorPow(const Vec4<T> &v, const Vec4<T> &e)
{
	return Vec4<T>(std::pow(v.x, e.x), std::pow(v.y, e.y), std::pow(v.z, e.z), std::pow(v.w, e.w));
}

template<typename T>
Vec2<T> VectorExp(const Vec2<T> &v)
{
	return Vec2<T>(std::exp(v.x), std::exp(v.y));
}

template<typename T>
Vec3<T> VectorExp(const Vec3<T> &v)
{
	return Vec3<T>(std::exp(v.x), std::exp(v.y), std::exp(v.z));
}

template<typename T>
Vec4<T> VectorExp(const Vec4<T> &v)
{
	return Vec4<T>(std::exp(v.x), std::exp(v.y), std::exp(v.z), std::exp(v.w));
}

template<typename T>
Vec4<T> LoadVector2(const Vec2<T> &v)
{
	return Vec4<T>(v.x, v.y, 0, 0);
}

template<typename T>
Vec4<T> LoadVector3(const Vec3<T> &v)
{
	return Vec4<T>(v.x, v.y, v.z, 0);
}

template<typename T>
void StoreVector2(Vec2<T> &v2, const Vec4<T> &v4)
{
	v2.x = v4.x;
	v2.y = v4.y;
}

template<typename T>
void StoreVector3(Vec3<T> &v3, const Vec4<T> &v4)
{
	v3.x = v4.x;
	v3.y = v4.y;
	v3.z = v4.z;
}

template<typename T>
Vec4<T> VectorLerp(const Vec4<T> &v0, const Vec4<T> &v1, T p)
{
	return (v0 * (1 - p) + v1 * p);
}
#endif // !VECTORMATH_H
