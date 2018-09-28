#pragma once
#ifndef MATRIXMATH_H
#define MATRIXMATH_H
#include "Matrix.h"
#include "Vector.h"

/*----------------*/
/* Matrix3x3 Math */
/*----------------*/
template<typename T>
T Matrix3X3Determinant(const Matrix3x3<T> &m)
{
	T det = m[0][0] * (m[1][1] * a[2][2] - a[1][2] * a[2][1]) -
		a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) +
		a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
	return det;
}

template<typename T>
Matrix3x3<T> Matrix3x3Inverse(const Matrix3x3<T> &m)
{
	Matrix3x3<T> mat = ZeroMatrix3x3<T>();
	T det = Matrix3X3Determinant(m);
	if (det != 0)
	{
		T inv_det = 1 / det;

		mat[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * inv_det;
		mat[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * inv_det;
		mat[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * inv_det;

		mat[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * inv_det;
		mat[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * inv_det;
		mat[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * inv_det;

		mat[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * inv_det;
		mat[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * inv_det;
		mat[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * inv_det;
	}

	return mat;
}

template<typename T>
Matrix3x3<T> Matrix3x3Transpose(const Matrix3x3<T> &m)
{
	Matrix3x3<T> mat = ZeroMatrix3x3<T>();
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			mat[i][j] = m[j][i];
		}
	}
	return mat;
}

template<typename T>
Matrix3x3<T> Matrix3x3Translation(const float &x, const float &y)
{
	Matrix3x3<T> mat = IdentityMatrix3x3<T>();
	mat[2][0] = x;
	mat[2][1] = y;

	mat[2][2] = 1;

	return mat;
}

template<typename T>
Matrix3x3<T> Matrix3x3TranslationFromVector(const Vec3<T> &v)
{
	Matrix3x3<T> mat = IdentityMatrix3x3<T>();
	mat[2][0] = v.x;
	mat[2][1] = v.y;

	mat[2][2] = 1;

	return mat;
}

template<typename T>
Matrix3x3<T> Matrix3x3Scaling(const float &scale_x, const float &scale_y)
{
	Matrix4x4<T> mat = ZeroMatrix3x3<T>();
	mat[0][0] = scale_x;
	mat[1][1] = scale_y;

	mat[2][2] = 1;

	return mat;
}

template<typename T>
Matrix3x3<T> Matrix3x3ScalingFromVector(const Vec3<T> &v)
{
	Matrix4x4<T> mat = ZeroMatrix3x3<T>();
	mat[0][0] = v.x;
	mat[1][1] = v.y;

	mat[2][2] = 1;

	return mat;
}

template<typename T>
Matrix3x3<T> Matrix3x3RotationOrigin(const T &theta_radian)
{
	Matrix4x4<T> mat = ZeroMatrix3x3<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][1] = sin_theta;

	mat[1][0] = -sin_theta;
	mat[1][1] = cos_theta;

	mat[2][2] = 1;

	return mat;
}

template<typename T>
Matrix3x3<T> Matrix3x3RotationPoint(const Vec3<T> &center, const T &theta_radian)
{
	Matrix4x4<T> mat = ZeroMatrix3x3<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][1] = sin_theta;

	mat[1][0] = -sin_theta;
	mat[1][1] = cos_theta;

	mat[2][0] = (1 - cos_theta) * center.x + center.y * sin_theta;
	mat[2][1] = -center.x * sin_theta + (1 - cos_theta) * center.y;

	mat[2][2] = 1;

	return mat;
}

/*----------------*/
/* Matrix4x4 Math */
/*----------------*/
template<typename T>
T Matrix4X4Determinant(const Matrix4x4<T> &m)
{
	T dst[4];
	dst[0] = m[1][1] * m[2][2] * m[3][3] +
		m[1][2] * m[2][3] * m[3][1] +
		m[1][3] * m[2][1] * m[3][2] -
		m[1][1] * m[2][3] * m[3][2] -
		m[1][2] * m[2][1] * m[3][3] -
		m[1][3] * m[2][2] * m[3][1];

	dst[1] = m[1][0] * m[2][3] * m[3][2] +
		m[1][2] * m[2][0] * m[3][3] +
		m[1][3] * m[2][2] * m[3][0] -
		m[1][0] * m[2][2] * m[3][3] -
		m[1][2] * m[2][3] * m[3][0] -
		m[1][3] * m[2][0] * m[3][2];

	dst[2] = m[1][0] * m[2][1] * m[3][3] +
		m[1][1] * m[2][3] * m[3][0] +
		m[1][3] * m[2][0] * m[3][1] -
		m[1][0] * m[2][3] * m[3][1] -
		m[1][1] * m[2][0] * m[3][3] -
		m[1][3] * m[2][1] * m[3][0];

	dst[3] = m[1][0] * m[2][2] * m[3][1] +
		m[1][1] * m[2][0] * m[3][2] +
		m[1][2] * m[2][1] * m[3][0] -
		m[1][0] * m[2][1] * m[3][2] -
		m[1][1] * m[2][2] * m[3][0] -
		m[1][2] * m[2][0] * m[3][1];

	T det = m[0][0] * dst[0] + m[0][1] * dst[1] + m[0][2] * dst[2] + m[0][3] * dst[3];

	return det;
}

template<typename T>
Matrix4x4<T> Matrix4x4Inverse(const Matrix4x4<T> &m)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();
	T det = Matrix4X4Determinant(m);

	if (det!= 0)
	{
		T inv_det = 1 / det;

		mat[0][1] = (m[0][1] * m[2][3] * m[3][2] +
			m[0][2] * m[2][1] * m[3][3] +
			m[0][3] * m[2][2] * m[3][1] -
			m[0][1] * m[2][2] * m[3][3] -
			m[0][2] * m[2][3] * m[3][1] -
			m[0][3] * m[2][1] * m[3][2]) * inv_det;

		mat[0][2] = (m[0][1] * m[1][2] * m[3][3] +
			m[0][2] * m[1][3] * m[3][1] +
			m[0][3] * m[1][1] * m[3][2] -
			m[0][1] * m[1][3] * m[3][2] -
			m[0][2] * m[1][1] * m[3][3] -
			m[0][3] * m[1][2] * m[3][1]) * inv_det;

		mat[0][3] = (m[0][1] * m[1][3] * m[2][2] +
			m[0][2] * m[1][1] * m[2][3] +
			m[0][3] * m[1][2] * m[2][1] -
			m[0][1] * m[1][2] * m[2][3] -
			m[0][2] * m[1][3] * m[2][1] -
			m[0][3] * m[1][1] * m[2][2]) * inv_det;

		mat[1][1] = (m[0][0] * m[2][2] * m[3][3] +
			m[0][2] * m[2][3] * m[3][0] +
			m[0][3] * m[2][0] * m[3][2] -
			m[0][0] * m[2][3] * m[3][2] -
			m[0][2] * m[2][0] * m[3][3] -
			m[0][3] * m[2][2] * m[3][0]) * inv_det;

		mat[1][2] = (m[0][0] * m[1][3] * m[3][2] +
			m[0][2] * m[1][0] * m[3][3] +
			m[0][3] * m[1][2] * m[3][0] -
			m[0][0] * m[1][2] * m[3][3] -
			m[0][2] * m[1][3] * m[3][0] -
			m[0][3] * m[1][0] * m[3][2]) * inv_det;

		mat[1][3] = (m[0][0] * m[1][2] * m[2][3] +
			m[0][2] * m[1][3] * m[2][0] +
			m[0][3] * m[1][0] * m[2][2] -
			m[0][0] * m[1][3] * m[2][2] -
			m[0][2] * m[1][0] * m[2][3] -
			m[0][3] * m[1][2] * m[2][0]) * inv_det;

		mat[2][1] = (m[0][0] * m[2][3] * m[3][1] +
			m[0][1] * m[2][0] * m[3][3] +
			m[0][3] * m[2][1] * m[3][0] -
			m[0][0] * m[2][1] * m[3][3] -
			m[0][1] * m[2][3] * m[3][0] -
			m[0][3] * m[2][0] * m[3][1]) * inv_det;

		mat[2][2] = (m[0][0] * m[1][1] * m[3][3] +
			m[0][1] * m[1][3] * m[3][0] +
			m[0][3] * m[1][0] * m[3][1] -
			m[0][0] * m[1][3] * m[3][1] -
			m[0][1] * m[1][0] * m[3][3] -
			m[0][3] * m[1][1] * m[3][0]) * inv_det;

		mat[2][3] = (m[0][0] * m[1][3] * m[2][1] +
			m[0][1] * m[1][0] * m[2][3] +
			m[0][3] * m[1][1] * m[2][0] -
			m[0][0] * m[1][1] * m[2][3] -
			m[0][1] * m[1][3] * m[2][0] -
			m[0][3] * m[1][0] * m[2][1]) * inv_det;

		mat[3][1] = (m[0][0] * m[2][1] * m[3][2] +
			m[0][1] * m[2][2] * m[3][0] +
			m[0][2] * m[2][0] * m[3][1] -
			m[0][0] * m[2][2] * m[3][1] -
			m[0][1] * m[2][0] * m[3][2] -
			m[0][2] * m[2][1] * m[3][0]) * inv_det;

		mat[3][2] = (m[0][0] * m[1][2] * m[3][1] +
			m[0][1] * m[1][0] * m[3][2] +
			m[0][2] * m[1][1] * m[3][0] -
			m[0][0] * m[1][1] * m[3][2] -
			m[0][1] * m[1][2] * m[3][0] -
			m[0][2] * m[1][0] * m[3][1]) * inv_det;

		mat[3][3] = (m[0][0] * m[1][1] * m[2][2] +
			m[0][1] * m[1][2] * m[2][0] +
			m[0][2] * m[1][0] * m[2][1] -
			m[0][0] * m[1][2] * m[2][1] -
			m[0][1] * m[1][0] * m[2][2] -
			m[0][2] * m[1][1] * m[2][0]) * inv_det;
	}

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4Transpose(const Matrix4x4<T> &m)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			mat[i][j] = m[j][i];
		}
	}
	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4Translation(const T &x, const T &y, const T &z)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4TranslationFromVector(const Vec4<T> &v)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[3][0] = v.x;
	mat[3][1] = v.y;
	mat[3][2] = v.z;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4Scaling(const float &scale_x, const float &scale_y, const float &scale_z)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();
	mat[0][0] = scale_x;
	mat[1][1] = scale_y;
	mat[2][2] = scale_z;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4ScalingFromVector(const Vec4<T> &v)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();
	mat[0][0] = v.x;
	mat[1][1] = v.y;
	mat[2][2] = v.z;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4RotationX(const T &theta_radian)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = 1;
	mat[1][1] = cos_theta;
	mat[1][2] = sin_theta;
	mat[2][1] = -sin_theta;
	mat[2][2] = cos_theta;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4RotationY(const T &theta_radian)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][2] = -sin_theta;
	mat[1][1] = 1;
	mat[2][0] = sin_theta;
	mat[2][2] = cos_theta;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4RotationZ(const T &theta_radian)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][1] = sin_theta;
	mat[1][0] = -sin_theta;
	mat[1][1] = cos_theta;
	mat[2][2] = 1;

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4RotationAxis(const Vec4<T> &axi, const T &theta_radian)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);
	T sub_cos_theta = 1 - cos_theta;

	T x = axi.x;
	T y = axi.y;
	T z = axi.z;

	mat[0][0] = cos_theta + sub_cos_theta * x * x;
	mat[0][1] = sub_cos_theta * x * y + sin_theta * z;
	mat[0][2] = sub_cos_theta * x * z - sin_theta * y;

	mat[1][0] = sub_cos_theta * x * y - sin_theta * z;
	mat[1][1] = cos_theta + sub_cos_theta * y * y;
	mat[1][2] = sub_cos_theta * y * z + sin_theta * x;

	mat[2][0] = sub_cos_theta * x * z + sin_theta * y;
	mat[2][1] = sub_cos_theta * y * z - sin_theta * x;
	mat[2][2] = cos_theta + sub_cos_theta * z * z;

	mat[3][3] = 1;

	return mat;
}

//ComputeViewMatrix
template<typename T>
Matrix4x4<T> Matrix4x4LookAt(const Vec4<T> &eye_pos, const Vec4<T> &focus_pos, 
	const Vec4<T> &up_dir = Vec4<T>(0,1,0,0))
{
	Vec4<T> view_forward_dir = focus_pos - eye_pos;
	view_forward_dir.Normalize();

	Vec4<T> view_right_dir = Cross(up_dir, view_forward_dir);
	view_right_dir.Normalize();

	Vec4<T> view_up_dir = Cross(view_forward_dir, view_right_dir);
	view_up_dir.Normalize;

	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	mat[0][0] = view_right_dir.x;
	mat[1][0] = view_right_dir.y;
	mat[2][0] = view_right_dir.z;

	mat[0][1] = view_up_dir.x;
	mat[1][1] = view_up_dir.y;
	mat[2][1] = view_up_dir.z;

	mat[0][2] = view_forward_dir.x;
	mat[1][2] = view_forward_dir.y;
	mat[2][2] = view_forward_dir.z;

	mat[3][0] = -Dot(eye_pos, view_right_dir);
	mat[3][1] = -Dot(eye_pos, view_up_dir);
	mat[3][2] = -Dot(eye_pos, view_forward_dir);

	mat[3][3] = 1;
}

template<typename T>
Matrix4x4<T> Matrix4x4PerspectiveFov(const float &fov_y, const float &aspect_ratio,
	const float &near_z, const float &far_z)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	T tan_theta = static_cast<T>(std::tan(fov_y * 0.5));
	T inv_tan = 1 / tan_theta;

	T beta = far_z / (far_z - near_z);

	mat[0][0] = inv_tan * (1 / aspect_ratio);
	mat[1][1] = inv_tan;
	mat[2][2] = beta;
	mat[3][2] = - near_z * beta;

	mat[2][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4PerspectiveOffCenter(const float &right, const float &left,
	const float &top, const float &bottom,
	const float &near_z, const float &far_z)
{
	Matrix4x4<T> mat = ZeroMatrix4x4<T>();

	T alpha = 1 / (right - left);
	T beta = 1 / (top - bottom);
	T gamma = far_z / (far_z - near_z);

	mat[0][0] = 2 * near_z * alpha;
	mat[2][0] = -(right + left) * alpha;
	mat[1][1] = 2 * near_z * beta;
	mat[2][1] = -(top + bottom) * beta;
	mat[2][2] = gamma;
	mat[3][2] = -near_z * gamma;

	mat[2][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> MatrixTransformation(const Vec4<T> &scaling,
	const Vec4<T> &rotation_quater,
	const Vec4<T> &translation)
{
	Matrix4x4<T> temp = ZeroMatrix4x4<T>();

	temp = Matrix4x4ScalingFromVector<T>(scaling) * QuaternionToRotationMatrix(rotation_quater) * Matrix4x4TranslationFromVector(translation);

	return temp;
}

#endif // !MATRIXMATH_H
