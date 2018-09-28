#pragma once
#ifndef MATRIX_H
#define MATRIX_H
#include "PCH.h"
#include "Vector.h"

/*-----------*/
/* Matrix3x3 */
/*-----------*/
template<typename T>
class Matrix3x3
{
	template<typename U>
	friend Matrix3x3<U> operator*(const Matrix3x3<U> &lhs, const Matrix3x3<U> &rhs);

	template<typename U>
	friend std::ostream& operator<<(std::ostream &lhs, const Matrix3x3<U> &rhs);

public:
	Matrix3x3() {}
	~Matrix3x3() {}
	Matrix3x3(const Matrix3x3 &m)
	{
		matrix[0][0] = m.matrix[0][0];
		matrix[0][1] = m.matrix[0][1];
		matrix[0][2] = m.matrix[0][2];

		matrix[1][0] = m.matrix[1][0];
		matrix[1][1] = m.matrix[1][1];
		matrix[1][2] = m.matrix[1][2];

		matrix[2][0] = m.matrix[2][0];
		matrix[2][1] = m.matrix[2][1];
		matrix[2][2] = m.matrix[2][2];
	}

	const Matrix3x3 operator=(const Matrix3x3 &m)
	{
		matrix[0][0] = m.matrix[0][0];
		matrix[0][1] = m.matrix[0][1];
		matrix[0][2] = m.matrix[0][2];

		matrix[1][0] = m.matrix[1][0];
		matrix[1][1] = m.matrix[1][1];
		matrix[1][2] = m.matrix[1][2];

		matrix[2][0] = m.matrix[2][0];
		matrix[2][1] = m.matrix[2][1];
		matrix[2][2] = m.matrix[2][2];

		return *this;
	}

	Matrix3x3(T arr[3][3])
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			for (uint8_t j = 0; j < 3; j++)
			{
				matrix[i][j] = arr[i][j];
			}
		}
	}

	Matrix3x3(T a00, T a01, T a02,
		T a10, T a11, T a12,
		T a20, T a21, T a22)
	{
		matrix[0][0] = a00;
		matrix[0][1] = a01;
		matrix[0][2] = a02;

		matrix[1][0] = a10;
		matrix[1][1] = a11;
		matrix[1][2] = a12;

		matrix[2][0] = a20;
		matrix[2][1] = a21;
		matrix[2][2] = a22;
	}

	const T* operator [] (uint8_t i) const { return matrix[i]; }
	T* operator [] (uint8_t i) { return matrix[i]; }

	Matrix3x3 &operator*=(const Matrix3x3 &rhs)
	{
		T mat[3][3];
		for (uint8_t i = 0; i < 3; i++)
		{
			for (uint8_t j = 0; j < 3; j++)
			{
				mat[i][j] = lhs[i][0] * rhs[0][j] +
					lhs[i][1] * rhs[1][j] +
					lhs[i][2] * rhs[2][j];
			}
		}

		matrix = mat;
		return *this;
	}

	T matrix[3][3] = { 0 };
};

template<typename U>
Matrix3x3<U> operator*(const Matrix3x3<U> &lhs, const Matrix3x3<U> &rhs)
{
	Matrix3x3<U> mat;
	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = 0; j < 3; j++)
		{
			mat[i][j] = lhs[i][0] * rhs[0][j] +
				lhs[i][1] * rhs[1][j] +
				lhs[i][2] * rhs[2][j];
		}
	}

	return mat;
}

template<typename U>
std::ostream& operator<<(std::ostream &lhs, const Matrix3x3<U> &rhs)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = 0; j < 3; j++)
		{
			lhs << rhs[i][j] << " ";
		}
		lhs << std::endl;
	}
	return lhs;
}

template<typename U>
Matrix3x3<U> IdentityMatrix3x3()
{
	Matrix3x3<U> mat;
	mat[0][0] = 1.0f;
	mat[1][1] = 1.0f;
	mat[2][2] = 1.0f;
	return mat;
}

template<typename U>
Matrix3x3<U> ZeroMatrix3x3()
{
	Matrix3x3<U> mat;
	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = 0; j < 3; j++)
		{
			mat[i][j] = 0;
		}
	}
	return mat;
}

using Matrix3x3f = Matrix3x3<float>;

/*-----------*/
/* Matrix4x4 */
/*-----------*/
template<typename T>
class Matrix4x4
{
	template<typename U>
	friend Matrix4x4<U> operator*(const Matrix4x4<U> &lhs, const Matrix4x4<U> &rhs);

	template<typename U>
	friend std::ostream& operator<<(std::ostream &lhs, const Matrix4x4<U> &rhs);

public:
	Matrix4x4() {};
	~Matrix4x4() {};
	Matrix4x4(const Matrix4x4 &m)
	{
		matrix[0][0] = m.matrix[0][0];
		matrix[0][1] = m.matrix[0][1];
		matrix[0][2] = m.matrix[0][2];
		matrix[0][3] = m.matrix[0][3];

		matrix[1][0] = m.matrix[1][0];
		matrix[1][1] = m.matrix[1][1];
		matrix[1][2] = m.matrix[1][2];
		matrix[1][3] = m.matrix[1][3];

		matrix[2][0] = m.matrix[2][0];
		matrix[2][1] = m.matrix[2][1];
		matrix[2][2] = m.matrix[2][2];
		matrix[2][3] = m.matrix[2][3];

		matrix[3][0] = m.matrix[3][0];
		matrix[3][1] = m.matrix[3][1];
		matrix[3][2] = m.matrix[3][2];
		matrix[3][3] = m.matrix[3][3];
	}

	const Matrix4x4 &operator=(const Matrix4x4 &m)
	{
		matrix[0][0] = m.matrix[0][0];
		matrix[0][1] = m.matrix[0][1];
		matrix[0][2] = m.matrix[0][2];
		matrix[0][3] = m.matrix[0][3];

		matrix[1][0] = m.matrix[1][0];
		matrix[1][1] = m.matrix[1][1];
		matrix[1][2] = m.matrix[1][2];
		matrix[1][3] = m.matrix[1][3];

		matrix[2][0] = m.matrix[2][0];
		matrix[2][1] = m.matrix[2][1];
		matrix[2][2] = m.matrix[2][2];
		matrix[2][3] = m.matrix[2][3];

		matrix[3][0] = m.matrix[3][0];
		matrix[3][1] = m.matrix[3][1];
		matrix[3][2] = m.matrix[3][2];
		matrix[3][3] = m.matrix[3][3];

		return *this;
	}

	Matrix4x4(T arr[4][4])
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			for (uint8_t j = 0; j < 4; j++)
			{
				matrix[i][j] = arr[i][j];
			}
		}
	}

	Matrix4x4(T a00, T a01, T a02, T a03,
		T a10, T a11, T a12, T a13,
		T a20, T a21, T a22, T a23,
		T a30, T a31, T a32, T a33)
	{
		matrix[0][0] = a00;
		matrix[0][1] = a01;
		matrix[0][2] = a02;
		matrix[0][3] = a03;

		matrix[1][0] = a10;
		matrix[1][1] = a11;
		matrix[1][2] = a12;
		matrix[1][3] = a13;

		matrix[2][0] = a20;
		matrix[2][1] = a21;
		matrix[2][2] = a22;
		matrix[2][3] = a23;

		matrix[3][0] = a30;
		matrix[3][1] = a31;
		matrix[3][2] = a32;
		matrix[3][3] = a33;
	}

	const T* operator [] (uint8_t i) const { return matrix[i]; }
	T* operator [] (uint8_t i) { return matrix[i]; }

	Matrix4x4 &operator*=(const Matrix4x4 &rhs)
	{
		T mat[4][4];
		for (uint8_t i = 0; i < 4; i++)
		{
			for (uint8_t j = 0; j < 4; j++)
			{
				mat[i][j] = lhs[i][0] * rhs[0][j] +
					lhs[i][1] * rhs[1][j] +
					lhs[i][2] * rhs[2][j] +
					lhs[i][3] * rhs[3][j];
			}
		}
		
		matrix = mat;
		return *this;
	}

	T matrix[4][4] = { 0 };
};

template<typename U>
Matrix4x4<U> operator*(const Matrix4x4<U> &lhs, const Matrix4x4<U> &rhs)
{
	Matrix4x4<U> mat;
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			mat[i][j] = lhs[i][0] * rhs[0][j] +
				lhs[i][1] * rhs[1][j] +
				lhs[i][2] * rhs[2][j] +
				lhs[i][3] * rhs[3][j];
		}
	}

	return mat;
}

template<typename U>
std::ostream& operator<<(std::ostream &lhs, const Matrix4x4<U> &rhs)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			lhs << rhs[i][j] << " ";
		}
		lhs << std::endl;
	}
	return lhs;
}

template<typename U>
Matrix4x4<U> IdentityMatrix4x4()
{
	Matrix4x4<U> mat;
	mat[0][0] = 1.0f;
	mat[1][1] = 1.0f;
	mat[2][2] = 1.0f;
	mat[3][3] = 1.0f;
	return mat;
}

template<typename U>
Matrix4x4<U> ZeroMatrix4x4()
{
	Matrix4x4<U> mat;
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			mat[i][j] = 0;
		}
	}
	return mat;
}

using Matrix4x4f = Matrix4x4<float>;
#endif // !MATRIX_H
