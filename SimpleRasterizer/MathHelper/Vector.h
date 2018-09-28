#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include "PCH.h"

template<typename T>
static inline void Swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

/*------*/
/* Vec2 */
/*------*/
template<typename T>
struct Vec2
{
	/*--------------*/
	/*  friend function   */
	/*--------------*/
	template<typename U>
	friend std::ostream& operator << (std::ostream &lhs, const Vec2<U> &rhs);

	template<typename U>
	friend std::istream& operator >> (std::istream &lhs, const Vec2<U> &rhs);

	template<typename U>
	friend Vec2<U> operator * (const U &lhs, const Vec2<U> &rhs);

	union
	{
		struct { T x, y; };
		struct { T u, v; };
		T data[2];
	};

	/*--------------*/
	/* Constructors */
	/*--------------*/
	Vec2() {};
	Vec2(const T &_x) : x(_x), y(_x) {};
	Vec2(const T &_x, const T &_y) :x(_x), y(_y) {};
	Vec2(const T *t) : x(t[0]), y(t[1]) {};
	Vec2(const Vec2 &v) : x(v.x), y(v.y) {};
	const Vec2 &operator = (const Vec2 &v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	/*-------------------*/
	/* Basic Vector Math */
	/*-------------------*/
	Vec2 operator + (const Vec2 &rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator - (const Vec2 &rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	const Vec2 &operator += (const Vec2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	const Vec2 &operator -= (const Vec2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	/*-------------------*/
	/* Basic Scalar Math */
	/*-------------------*/
	Vec2 operator + (const T &rhs) const
	{
		return Vec2(x + rhs, y + rhs);
	}

	Vec2 operator - (const T &rhs) const
	{
		return Vec2(x - rhs, y - rhs);
	}

	Vec2 operator * (const T &rhs) const
	{
		return Vec2(x * rhs, y * rhs);
	}

	Vec2 operator / (const T &rhs) const
	{
		return Vec2(x / rhs, y / rhs);
	}

	const Vec2 &operator += (const T &rhs)
	{
		x += rhs;
		y += rhs;
		return *this;
	}

	const Vec2 &operator -= (const T &rhs)
	{
		x -= rhs;
		y -= rhs;
		return *this;
	}

	const Vec2 &operator *= (const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	const Vec2 &operator /= (const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	/*------------*/
	/* Other Math */
	/*------------*/
	bool operator == (const Vec2 &rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	bool operator != (const Vec2 &rhs) const
	{
		return !(*this == rhs);
	}

	T LengthSq() const
	{
		return x * x + y * y;
	}

	T Length() const
	{
		return sqrt(LengthSq());
	}

	void Normalize()
	{
		*this /= Length();
	}
};

/*-----------------------*/
/* Vec2 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline U Dot(const Vec2<U> &lhs, const Vec2<U> &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename U>
inline U Cross(const Vec2<U> &lhs, const Vec2<U> &rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<typename U>
inline Vec2<U> Normalize(const Vec2<U> &lhs)
{
	U len = lhs.Length();
	return lhs / len;
}

template<typename U>
std::ostream& operator << (std::ostream &lhs, const Vec2<U> &rhs)
{
	lhs << rhs.x << " " << rhs.y;
	return lhs;
}

template<typename U>
std::istream& operator >> (std::istream &lhs, const Vec2<U> &rhs)
{
	lhs >> rhs.x >> rhs.y;
	return lhs;
}

template<typename U>
Vec2<U> operator * (const U &lhs, const Vec2<U> &rhs)
{
	return rhs * lhs;
}

/*------*/
/* Vec3 */
/*------*/
template<typename T>
struct Vec3
{
	/*----------------------*/
	/*   friend function   */
	/*---------------------*/
	template<typename U>
	friend std::ostream& operator<<(std::ostream &lhs, const Vec3<U> &rhs);

	template<typename U>
	friend std::istream& operator>>(std::istream &lhs, Vec3<U> &rhs);

	template<typename U>
	friend Vec3<U> operator*(const float &lhs, Vec3<U> &rhs);

	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		T data[3];
	};

	/*--------------*/
	/* Constructors */
	/*--------------*/
	Vec3() {};
	Vec3(const T &_x) :x(_x), y(_x), z(_x) {};
	Vec3(const T &_x, const T &_y, const T &_z) :x(_x), y(_y), z(_z) {};
	Vec3(const T *t) : x(t[0]), y(t[1]), t(t[2]) {};
	Vec3(const Vec3 &v) :x(v.x), y(v.y), z(v.z) {};
	const Vec3 &operator = (const Vec3 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	/*-------------------*/
	/* Basic Vector Math */
	/*-------------------*/
	Vec3 operator + (const Vec3 &rhs) const
	{
		return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vec3 operator - (const Vec3 &rhs) const
	{
		return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	const Vec3 &operator += (const Vec3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	const Vec3 &operator -= (const Vec3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/*-------------------*/
	/* Basic Scalar Math */
	/*-------------------*/
	Vec3 operator + (const T &rhs) const
	{
		return Vec3(x + rhs, y + rhs, z + rhs);
	}

	Vec3 operator -(const T &rhs) const
	{
		return Vec3(x - rhs, y - rhs, z - rhs);
	}

	Vec3 operator * (const T &rhs) const
	{
		return Vec3(x * rhs, y * rhs, z * rhs);
	}

	Vec3 operator / (const T &rhs) const
	{
		return Vec3(x / rhs, y / rhs, z / rhs);
	}

	const Vec3 &operator += (const T &rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	const Vec3 &operator -= (const T &rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	const Vec3 &operator *= (const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	const Vec3 &operator /= (const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	/*------------*/
	/* Other Math */
	/*------------*/
	bool operator == (const Vec3 &rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	bool operator != (const Vec3 &rhs) const
	{
		return !(*this == rhs);
	}

	T LengthSq() const
	{
		return x * x + y * y + z * z;
	}

	T Length() const
	{
		return sqrt(LengthSq());
	}

	void Normalize()
	{
		*this /= Length();
	}
};

/*-----------------------*/
/* Vec3 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline U Dot(const Vec3<U> &lhs, const Vec3<U> &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename U>
inline Vec3<U> Cross(const Vec3<U> &lhs, const Vec3<U> &rhs)
{
	return Vec3<U>(lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x);
}

template<typename U>
inline Vec3<U> Normalize(const Vec3<U> &lhs)
{
	U len = lhs.Length();
	return lhs / len;
}

template<typename U>
inline Vec3<U> Mul(const Vec3<U> &lhs, const Vec3<U> &rhs)
{
	return Vec3<U>(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

template<typename T>
std::ostream& operator<<(std::ostream &lhs, const Vec3<T> &rhs)
{
	lhs << rhs.x << " " << rhs.y << " " << rhs.z;
	return lhs;
}

template<typename T>
std::istream& operator>>(std::istream &lhs, Vec3<T> &rhs)
{
	lhs >> rhs.x >> rhs.y >> rhs.z;
	if (!lhs)
	{
		rhs = Vec3<T>();
		std::cout << "Vec3 cin failed" << std::endl;
	}
	return lhs;
}

template<typename U>
Vec3<U> operator*(const float &lhs, Vec3<U> &rhs)
{
	return rhs * lhs;
}

/*------*/
/* Vec4 */
/*------*/
template<typename T>
struct Vec4
{
	/*--------------*/
	/*   friend function   */
	/*--------------*/
	template<typename U>
	friend std::ostream& operator<<(std::ostream &lhs, const Vec4<U> &rhs);

	template<typename U>
	friend std::istream& operator>>(std::istream &lhs, Vec4<U> &rhs);

	template<typename U>
	friend Vec4<U> operator*(const U &lhs, Vec4<U> &rhs);

	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T data[4];
	};

	/*--------------*/
	/* Constructors */
	/*--------------*/
	Vec4() {};
	Vec4(const T &_x) :x(_x), y(_x), z(_x), w(_x) {};
	Vec4(const T &_x, const T &_y, const T &_z, const T &_w) : x(_x), y(_y), z(_z), w(_w) {};
	Vec4(const T *t) :x(t[0]), y(t[1]), z(t[2]), w(t[3]) {};
	Vec4(const Vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {};
	const Vec4 &operator = (const Vec4 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	/*-------------------*/
	/* Basic Vector Math */
	/*-------------------*/
	Vec4 operator + (const Vec4 &rhs) const
	{
		return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	Vec4 operator - (const Vec4 &rhs) const
	{
		return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	const Vec4 &operator += (const Vec4 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	const Vec4 &operator -= (const Vec4 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	/*-------------------*/
	/* Basic Scalar Math */
	/*-------------------*/
	Vec4 operator + (const T &rhs) const
	{
		return Vec4(x + rhs, y + rhs, z + rhs, w + rhs);
	}

	Vec4 operator - (const T &rhs) const
	{
		return Vec4(x - rhs, y - rhs, z - rhs, w - rhs);
	}

	Vec4 operator * (const T &rhs) const
	{
		return Vec4(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	Vec4 operator / (const T &rhs) const
	{
		return Vec4(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	const Vec4 &operator += (const T &rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;
		return *this;
	}

	const Vec4 &operator -= (const T &rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;
		return *this;
	}

	const Vec4 &operator *= (const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	const Vec4 &operator /= (const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	/*------------*/
	/* Other Math */
	/*------------*/
	bool operator == (const Vec4 &rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	bool operator != (const Vec4 &rhs) const
	{
		return !(*this == rhs);
	}

	T LengthSq() const
	{
		return x * x + y * y + z * z + w * w;
	}

	T Length() const
	{
		return sqrt(LengthSq());
	}

	void Normalize()
	{
		*this /= Length();
	}
};

/*-----------------------*/
/* Vec4 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline U Dot(const Vec4<U> &lhs, const Vec4<U> &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template<typename U>
inline Vec4<U> Mul(const Vec4<U> &lhs, const Vec4<U> &rhs)
{
	return Vec4<U>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

template<typename U>
inline Vec4<U> Normalize(const Vec4<U> &rhs)
{
	U len = rhs.Length();
	return rhs / len;
}

template<typename T>
std::ostream& operator<<(std::ostream &lhs, const Vec4<T> &rhs)
{
	lhs << rhs.x << " " << rhs.y << " " << rhs.z << " " << rhs.w;
	return lhs;
}

template<typename T>
std::istream& operator>>(std::istream &lhs, Vec4<T> &rhs)
{
	lhs >> rhs.x >> rhs.y >> rhs.z >> rhs.w;
	if (!lhs)
	{
		rhs = Vec4<T>();
		std::cout << "Vec3 cin failed" << std::endl;
	}
	return lhs;
}

template<typename U>
Vec4<U> operator*(const U &lhs, Vec4<U> &rhs)
{
	return rhs * lhs;
}

using Vec2I = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec3f = Vec3<float>;
using Vec4f = Vec4<float>;
#endif // !VECTOR_H
