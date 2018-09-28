#ifndef QUATERNIONMATH_H
#define QUATERNIONMATH_H
#include "Matrix.h"
#include "Vector.h"

template<typename T>
Vec4<T> QuaternionRotationAxis(const Vec4<T> &axis, const T &theta)
{
	Vec4<T> norm_axis = Normalize(axis);

	float cos_theta = std::cos(theta);
	float sin_theta = std::sin(theta);

	Vec4<T> temp;
	temp.x = norm_axis.x * sin_theta;
	temp.y = norm_axis.y * sin_theta;
	temp.z = norm_axis.z * sin_theta;
	temp.w = cos_theta;

	return temp;
}

template<typename T>
Matrix4x4<T> QuaternionToRotationMatrix(const Vec4<T> &quater)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	mat[0][0] = 1 - 2 * quater.y * quater.y - 2 * quater.z * quater.z;
	mat[0][1] = 2 * quater.x * quater.y + 2 * quater.z * quater.w;
	mat[0][2] = 2 * quater.x * quater.z - 2 * quater.y * quater.w;

	mat[1][0] = 2 * quater.x * quater.y - 2 * quater.z * quater.w;
	mat[1][1] = 1 - 2 * quater.x * quater.x - 2 * quater.z * quater.z;
	mat[1][2] = 2 * quater.y * quater.z + 2 * quater.x * quater.w;

	mat[2][0] = 2 * quater.x * quater.z + 2 * quater.y * quater.w;
	mat[2][1] = 2 * quater.y * quater.z - 2 * quater.x * quater.w;
	mat[2][2] = 1 - 2 * quater.x * quater.x - 2 * quater.y * quater.y;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Vec4<T> RotationMatrixToQuaternion(const Matrix4x4<T> &m)
{
	Vec4<T> temp;

	T trace = m[0][0] + m[1][1] + m[2][2];
	if (trace > 0)
	{
		T s = std::sqrt(trace + 1) * 2;

		temp.w = 0.25 * s;

		T inv_s = 1 / s;

		temp.x = (m[1][2] - m[2][1]) * inv_s;
		temp.y = (m[2][0] - m[0][2]) * inv_s;
		temp.z = (m[0][1] - m[1][0]) * inv_s;
	}
	else if ((m[0][0] > m[1][1])&(m[0][0] > m[2][2]))
	{
		T s = std::sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]) * 2; // S=4*qx 

		temp.x = 0.25 * s;

		T inv_s = 1 / s;

		temp.w = (m[1][2] - m[2][1]) * inv_s;
		temp.y = (m[1][0] + m[0][1]) * inv_s;
		temp.z = (m[2][0] + m[0][2]) * inv_s;
 	}
	else if (m[1][1] > m[2][2])
	{
		T s = std::sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]) * 2; // S=4*qy

		temp.y = 0.25 * s;

		T inv_s = 1 / s;

		temp.w = (m[2][0] - m[0][2]) * inv_s;
		temp.x = (m[1][0] + m[0][1]) * inv_s;
		temp.z = (m[2][1] + m[1][2]) * inv_s;
	}
	else
	{
		T s = std::sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]) * 2; // S=4*qy

		temp.z = 0.25 * s;

		T inv_s = 1 / s;

		temp.w = (m[0][1] - m[1][0]) * inv_s;
		temp.x = (m[2][0] + m[0][2]) * inv_s;
		temp.y = (m[2][1] + m[1][2]) * inv_s;
	}

	return temp;
}

template<typename T>
Vec4<T> LerpAndNormalize(const Vec4<T> &quater0, const Vec4<T> &quater1, const T &t)
{
	return Normalize(quater0 * (1 - t)+ quater1 * t);
}

template<typename T>
Vec4<T> QuaternionSlerp(const Vec4<T> &quater0, const Vec4<T> &quater1, const T &t)
{
	Vec4<T> short_q1 = quater1;
	if ((quater0 - quater1).LengthSq() > (quater0 + quater1).LengthSq())
	{
		short_q1 *= -1;
	}

	T cos_theta = Dot(quater0, short_q1);

	if (cos_theta > (1.0f - 0.001f))
	{
		return LerpAndNormalize<T>(quater0, short_q1, t);
	}

	T theta = std::acos(cos_theta);

	T sin_theta = std::sin(theta);

	return (quater0 * std::sin((1 - t) * theta) + short_q1 * std::sin(t * theta) ) / sin_theta;
}

//chayige slerp
#endif // !QUATERNIONMATH_H