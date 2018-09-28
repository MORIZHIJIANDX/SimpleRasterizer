#pragma once
#ifndef CLIPPER_H
#define CLIPPER_H

#include "RenderMath.h"

enum class FragmentLayout
{
	BASE,
	EXTENSION0,
	EXTENSION1,
	EXTENSION2
};

struct Vertex
{
	Vertex() {};

	Vertex(const Vec3f &pPos, const Vec3f &pNormal, const Vec2f &pUV) : m_pos(pPos), m_normal(pNormal), m_uv(pUV)
	{
	
	}

	Vertex(const float &pPosX, const float &pPosY, const float &pPosZ, 
		const float &pNormalX, const float &pNormalY, const float &pNormalZ,
		const float &pUVX, const float &pUVY) :
		m_pos(pPosX, pPosY, pPosZ), m_normal(pNormalX, pNormalY, pNormalZ), m_uv(pUVX, pUVY)
	{

	}

	Vec3f m_pos;
	Vec3f m_normal;
	Vec2f m_uv;
	Vec4f pack0;
	Vec4f pack1;
};

struct Fragment
{
	Vec4f m_pos = Vec4f(0.0f);
	Vec3f m_normal = Vec3f(0.0f);
	Vec2f m_uv = Vec2f(0.0f);
	Vec4f pack0 = Vec4f(0.0f);
	Vec4f pack1 = Vec4f(0.0f);
};

struct Triangle
{
	Fragment m_vertex[3];
};

struct Viewport
{
	float m_top_leftx;
	float m_top_lefty;
	float m_width;
	float m_height;
};


static const std::bitset<6> Inside("0");

static const std::bitset<6> Left("1");
static const std::bitset<6> Right("10");

static const std::bitset<6> Bottom("100");
static const std::bitset<6> Top("1000");

static const std::bitset<6> Near("10000");
static const std::bitset<6> Far("100000");

using ClipperInterpolationFun = std::function<void(const Fragment &pFragment0, const Fragment &pFragment1, const float &t, Fragment &pDest)>;

inline void BaseInterpolationFun(const Fragment &pFragment0, const Fragment &pFragment1, const float &t, Fragment &pDest)
{
	pDest.m_normal = pFragment0.m_normal * (1 - t) + pFragment1.m_normal * t;
	pDest.m_uv = pFragment0.m_uv * (1 - t) + pFragment1.m_uv * t;
}

inline void Extension0InterpolationFun(const Fragment &pFragment0, const Fragment &pFragment1, const float &t, Fragment &pDest)
{
	pDest.m_normal = pFragment0.m_normal * (1 - t) + pFragment1.m_normal * t;
	pDest.m_uv = pFragment0.m_uv * (1 - t) + pFragment1.m_uv * t;
	pDest.pack0 = pFragment0.pack0 * (1 - t) + pFragment1.pack0 * t;
}

inline void Extension1InterpolationFun(const Fragment &pFragment0, const Fragment &pFragment1, const float &t, Fragment &pDest)
{
	pDest.m_normal = pFragment0.m_normal * (1 - t) + pFragment1.m_normal * t;
	pDest.m_uv = pFragment0.m_uv * (1 - t) + pFragment1.m_uv * t;
	pDest.pack0 = pFragment0.pack0 * (1 - t) + pFragment1.pack0 * t;
	pDest.pack1 = pFragment0.pack1 * (1 - t) + pFragment1.pack1 * t;
}

class Clipper
{
public:
	Clipper() 
	{
		m_inter_fun = BaseInterpolationFun;
	}
	~Clipper() {}

	void SetFragmentLayout(const FragmentLayout &layout)
	{
		switch (layout)
		{
		case FragmentLayout::BASE:
			m_inter_fun = BaseInterpolationFun;
			break;
		case FragmentLayout::EXTENSION0:
			m_inter_fun = Extension0InterpolationFun;
			break;
		case FragmentLayout::EXTENSION1:
			m_inter_fun = Extension1InterpolationFun;
			break;
		default:
			break;
		}
	}

	void Clip(Triangle **pTris, size_t &pSize)
	{
		std::vector<Triangle> clipped_tris;
		clipped_tris.reserve(pSize);

		for (size_t i = 0; i < pSize; i++)
		{
			Triangle curr_tri = (*pTris)[i];

			Fragment vertex0 = curr_tri.m_vertex[0];
			Fragment vertex1 = curr_tri.m_vertex[1];
			Fragment vertex2 = curr_tri.m_vertex[2];

			Vec4f v0 = vertex0.m_pos;
			Vec4f v1 = vertex1.m_pos;
			Vec4f v2 = vertex2.m_pos;

			std::bitset<6> clip_code[3];

			clip_code[0] = ComputeClipCode(v0);
			clip_code[1] = ComputeClipCode(v1);
			clip_code[2] = ComputeClipCode(v2);

			std::bitset<6> clip_code_and = clip_code[0] & clip_code[1] & clip_code[2];
			std::bitset<6> clip_code_or = clip_code[0] | clip_code[1] | clip_code[2];

			if (clip_code_and != Inside)
			{
				continue;
			}

			if (clip_code_or == Inside)
			{
				clipped_tris.emplace_back((*pTris)[i]);
				continue;
			}

			std::bitset<6> clip_code_xor = (clip_code[0] ^ clip_code[1]) | (clip_code[1] ^ clip_code[2]) | (clip_code[2] ^ clip_code[0]);

			std::vector<Triangle> unclipped_tris;
			std::vector<Triangle> curr_cliped_tirs;

			unclipped_tris.emplace_back(curr_tri);

			if ((clip_code_xor & Near) == Near)
			{
				for (size_t i = 0; i < unclipped_tris.size(); i++)
				{
					ClipPlane(unclipped_tris[i],
						[](const Vec4f &p) {
						return p.z < 0;
					},
						[](const Vec4f &p0, const Vec4f &p1) {
						return p0.z / (p0.z - p1.z);
					},
						[](Vec4f &p) {
						p.z = 0;
					}, curr_cliped_tirs);
				}

				if (FilterTriangles(curr_cliped_tirs, clipped_tris))
				{
					continue;
				}

				std::swap(curr_cliped_tirs, unclipped_tris);
				curr_cliped_tirs.clear();
			}

			if ((clip_code_xor & Far) == Far)
			{
				for (size_t i = 0; i < unclipped_tris.size(); i++)
				{
					ClipPlane(unclipped_tris[i],
						[](const Vec4f &p) {
						return p.z > p.w;
					},
						[](const Vec4f &p0, const Vec4f &p1)
					{
						return (p0.z - p0.w) / ((p0.z - p0.w) - (p1.z - p1.w));
					},
						[](Vec4f &p) {
						p.z = p.w;
					}, curr_cliped_tirs);
				}

				if (FilterTriangles(curr_cliped_tirs, clipped_tris))
				{
					continue;
				}

				std::swap(curr_cliped_tirs, unclipped_tris);
				curr_cliped_tirs.clear();
			}

			if ((clip_code_xor & Left) == Left)
			{
				for (size_t i = 0; i < unclipped_tris.size(); i++)
				{
					ClipPlane(unclipped_tris[i],
						[](const Vec4f &p) {
						return p.x < -p.w;
					},
						[](const Vec4f &p0, const Vec4f &p1)
					{
						return (p0.x + p0.w) / ((p0.x + p0.w) - (p1.x + p1.w));
					},
						[](Vec4f &p) {
						p.x = -p.w;
					}, curr_cliped_tirs);
				}

				if (FilterTriangles(curr_cliped_tirs, clipped_tris))
				{
					continue;
				}

				std::swap(curr_cliped_tirs, unclipped_tris);
				curr_cliped_tirs.clear();
			}

			if ((clip_code_xor & Right) == Right)
			{
				for (size_t i = 0; i < unclipped_tris.size(); i++)
				{
					ClipPlane(unclipped_tris[i],
						[](const Vec4f &p) {
						return p.x > p.w;
					},
						[](const Vec4f &p0, const Vec4f &p1)
					{
						return (p0.x - p0.w) / ((p0.x - p0.w) - (p1.x - p1.w));
					},
						[](Vec4f &p) {
						p.x = p.w;
					}, curr_cliped_tirs);
				}

				if (FilterTriangles(curr_cliped_tirs, clipped_tris))
				{
					continue;
				}

				std::swap(curr_cliped_tirs, unclipped_tris);
				curr_cliped_tirs.clear();
			}

			if ((clip_code_xor & Bottom) == Bottom)
			{
				for (size_t i = 0; i < unclipped_tris.size(); i++)
				{
					ClipPlane(unclipped_tris[i],
						[](const Vec4f &p) {
						return p.y < -p.w;
					},
						[](const Vec4f &p0, const Vec4f &p1)
					{
						return (p0.y + p0.w) / ((p0.y + p0.w) - (p1.y + p1.w));
					},
						[](Vec4f &p) {
						p.y = -p.w;
					}, curr_cliped_tirs);
				}

				if (FilterTriangles(curr_cliped_tirs, clipped_tris))
				{
					continue;
				}

				std::swap(curr_cliped_tirs, unclipped_tris);
				curr_cliped_tirs.clear();
			}

			if ((clip_code_xor & Top) == Top)
			{
				for (size_t i = 0; i < unclipped_tris.size(); i++)
				{
					ClipPlane(unclipped_tris[i],
						[](const Vec4f &p) {
						return p.y > p.w;
					},
						[](const Vec4f &p0, const Vec4f &p1)
					{
						return (p0.y - p0.w) / ((p0.y - p0.w) - (p1.y - p1.w));
					},
						[](Vec4f &p) {
						p.y = p.w;
					}, curr_cliped_tirs);
				}

				if (FilterTriangles(curr_cliped_tirs, clipped_tris))
				{
					continue;
				}

				std::swap(curr_cliped_tirs, unclipped_tris);
				curr_cliped_tirs.clear();
			}
		}

		size_t clipped_size = clipped_tris.size();
		if (clipped_size > pSize)
		{
			delete[](*pTris);
			(*pTris) = new Triangle[clipped_size];
		}
		pSize = clipped_size;
		std::memcpy((*pTris), clipped_tris.data(), pSize * sizeof(Triangle));
		
	}
private:
	std::bitset<6> ComputeClipCode(const Vec4f &pVertex)
	{
		std::bitset<6> clip_code;
		if (pVertex.x < -pVertex.w) //left
		{
			clip_code.set(0, true);
		}

		if (pVertex.x > pVertex.w) //right
		{
			clip_code.set(1, true);
		}

		if (pVertex.y < -pVertex.w) //bottom
		{
			clip_code.set(2, true);
		}

		if (pVertex.y > pVertex.w) //top
		{
			clip_code.set(3, true);
		}

		if (pVertex.z < 0) //near
		{
			clip_code.set(4, true);
		}

		if (pVertex.z > pVertex.w) //far
		{
			clip_code.set(5, true);
		}

		return clip_code;
	}

	template<typename PredictFun, typename SolveTFun, typename PositionFun>
	void ClipPlane(const Triangle &pTriangle, const PredictFun &pPreFun, const SolveTFun &pSolFun, const PositionFun &pPosFun, std::vector<Triangle> &pClipedTris)
	{
		Vec4f vertex_pos[3];

		vertex_pos[0] = pTriangle.m_vertex[0].m_pos;
		vertex_pos[1] = pTriangle.m_vertex[1].m_pos;
		vertex_pos[2] = pTriangle.m_vertex[2].m_pos;

		bool predicates[3];
		predicates[0] = pPreFun(vertex_pos[0]);
		predicates[1] = pPreFun(vertex_pos[1]);
		predicates[2] = pPreFun(vertex_pos[2]);

		if ((predicates[0] ^ predicates[1]) | (predicates[1] ^ predicates[2]) | (predicates[2] ^ predicates[0]))
		{
			std::vector<Fragment> clip_point;

			for (size_t i = 0; i < 3; i++)
			{
				size_t k = (i + 1) % 3;
				Vec4f pos0 = vertex_pos[i];
				Vec4f pos1 = vertex_pos[k];

				bool code0 = predicates[i];
				bool code1 = predicates[k];

				if (!code0 && !code1)
				{
					clip_point.emplace_back(pTriangle.m_vertex[k]);
				}
				else if (code0 && !code1)
				{
					float t = pSolFun(pos0, pos1);
					Vec4f new_point_pos = pos0 * (1 - t) + pos1 * t;
					pPosFun(new_point_pos);
					Fragment new_point;
					new_point.m_pos = new_point_pos;
					m_inter_fun(pTriangle.m_vertex[i], pTriangle.m_vertex[k], t, new_point);
					clip_point.emplace_back(new_point);
					clip_point.emplace_back(pTriangle.m_vertex[k]);
				}
				else if (!code0 && code1)
				{
					float t = pSolFun(pos0, pos1);
					Vec4f new_point_pos = pos0 * (1 - t) + pos1 * t;
					pPosFun(new_point_pos);
					Fragment new_point;
					new_point.m_pos = new_point_pos;
					m_inter_fun(pTriangle.m_vertex[i], pTriangle.m_vertex[k], t, new_point);
					clip_point.emplace_back(new_point);
				}
				else
				{
				}
			}

			for (size_t j = 0; j < clip_point.size() - 2; j++)
			{

				size_t n = (j + 1) % clip_point.size();
				size_t m = (j + 2) % clip_point.size();
				Triangle cliped_tri;
				cliped_tri.m_vertex[0] = clip_point[0];
				cliped_tri.m_vertex[1] = clip_point[n];
				cliped_tri.m_vertex[2] = clip_point[m];

				pClipedTris.emplace_back(cliped_tri);
			}
		}
		else
		{
			pClipedTris.emplace_back(pTriangle);
		}
	}

	bool FilterTriangles(std::vector<Triangle> &pCurrClippedTris, std::vector<Triangle> &pClippedTris)
	{
		std::vector<Triangle> tris_failed;
		for (size_t i = 0; i < pCurrClippedTris.size(); i++)
		{
			Triangle curr_tri = pCurrClippedTris[i];

			Vec4f v0 = curr_tri.m_vertex[0].m_pos;
			Vec4f v1 = curr_tri.m_vertex[1].m_pos;
			Vec4f v2 = curr_tri.m_vertex[2].m_pos;

			std::bitset<6> clip_code[3];

			clip_code[0] = ComputeClipCode(v0);
			clip_code[1] = ComputeClipCode(v1);
			clip_code[2] = ComputeClipCode(v2);

			std::bitset<6> clip_code_and = clip_code[0] & clip_code[1] & clip_code[2];
			std::bitset<6> clip_code_or = clip_code[0] | clip_code[1] | clip_code[2];

			if (clip_code_and != Inside)
			{
				continue;
			}

			if (clip_code_or == Inside)
			{
				pClippedTris.emplace_back(curr_tri);
				continue;
			}

			tris_failed.emplace_back(curr_tri);
		}

		pCurrClippedTris = tris_failed;

		if (pCurrClippedTris.size() != 0)
		{
			return false;
		}

		return true;
	}

	ClipperInterpolationFun m_inter_fun;
};
#endif 
