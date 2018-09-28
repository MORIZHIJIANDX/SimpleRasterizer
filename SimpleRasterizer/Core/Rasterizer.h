#pragma once
#ifndef RASTERIZER_H
#define RASTERIZER_H
#include "PCH.h"
#include "Clipper.h"
#include "Image.h"

static constexpr int blockSize = 16;

using RasterizerInterpolationFun = std::function<void(const Fragment &pFragment0, const Fragment &pFragment1, const Fragment &pFragment2,
	const float &t0, const float &t1, const float &t2, Fragment &pDest)>;

inline void BaseRaterFun(const Fragment &pFragment0, const Fragment &pFragment1, const Fragment &pFragment2,
	const float &t0, const float &t1, const float &t2, Fragment &pDest)
{
	pDest.m_pos = pFragment0.m_pos * t0 + pFragment1.m_pos * t1 + pFragment2.m_pos * t2;
	pDest.m_normal = pFragment0.m_normal * t0 + pFragment1.m_normal * t1 + pFragment2.m_normal * t2;
	pDest.m_uv = pFragment0.m_uv * t0 + pFragment1.m_uv * t1 + pFragment2.m_uv * t2;
}

inline void Extension0RaterFun(const Fragment &pFragment0, const Fragment &pFragment1, const Fragment &pFragment2,
	const float &t0, const float &t1, const float &t2, Fragment &pDest)
{
	pDest.m_pos = pFragment0.m_pos * t0 + pFragment1.m_pos * t1 + pFragment2.m_pos * t2;
	pDest.m_normal = pFragment0.m_normal * t0 + pFragment1.m_normal * t1 + pFragment2.m_normal * t2;
	pDest.m_uv = pFragment0.m_uv * t0 + pFragment1.m_uv * t1 + pFragment2.m_uv * t2;
	pDest.pack0 = pFragment0.pack0 * t0 + pFragment1.pack0 * t1 + pFragment2.pack0 * t2;
}

inline void Extension1RaterFun(const Fragment &pFragment0, const Fragment &pFragment1, const Fragment &pFragment2,
	const float &t0, const float &t1, const float &t2, Fragment &pDest)
{
	pDest.m_pos = pFragment0.m_pos * t0 + pFragment1.m_pos * t1 + pFragment2.m_pos * t2;
	pDest.m_normal = pFragment0.m_normal * t0 + pFragment1.m_normal * t1 + pFragment2.m_normal * t2;
	pDest.m_uv = pFragment0.m_uv * t0 + pFragment1.m_uv * t1 + pFragment2.m_uv * t2;
	pDest.pack0 = pFragment0.pack0 * t0 + pFragment1.pack0 * t1 + pFragment2.pack0 * t2;
	pDest.pack1 = pFragment0.pack1 * t0 + pFragment1.pack1 * t1 + pFragment2.pack1 * t2;
}

struct EdgeEquation
{
	int i, j, k, value;

	EdgeEquation() {};

	EdgeEquation(const Vec2I &p0, const Vec2I &p1, const Vec2I &p)
	{
		i = p0.y - p1.y;
		j = p1.x - p0.x;
		k = p0.x * p1.y - p0.y * p1.x;
		value = p.x * i + p.y * j + k;
	}

	void incrementX(const int &step = 1)
	{
		value += step * i;
	}

	void incrementY(const int &step = 1)
	{
		value += step * j;
	}
};

struct EdgeEquationSet
{
	EdgeEquation e0, e1, e2;

	EdgeEquationSet() {};

	EdgeEquationSet(const Vec2I &v0, const Vec2I &v1, const Vec2I &v2, const Vec2I &p)
	{
		e0 = EdgeEquation(v1, v2, p);
		e1 = EdgeEquation(v2, v0, p);
		e2 = EdgeEquation(v0, v1, p);
	}

	void incrementX(const int &step = 1)
	{
		e0.incrementX(step);
		e1.incrementX(step);
		e2.incrementX(step);
	}

	void incrementY(const int &step = 1)
	{
		e0.incrementY(step);
		e1.incrementY(step);
		e2.incrementY(step);
	}

	bool evaluate()
	{
		return e0.value >= 0 && e1.value >= 0 && e2.value >= 0;
	}
};

inline void RenderInsideBlock(const RasterizerInterpolationFun &pFun, const Triangle &pTriangle, const float (&pInvCamZ)[3], const EdgeEquationSet &pSet, const int &pArea, const int &pX, const int &pY,
	std::shared_ptr<ExtensionImage<float>> pDepthBuffer, std::vector<Fragment> &pFragments, std::vector<Vec2I> &pFragmentIndexes)
{
	EdgeEquationSet blockYSet = pSet;
	EdgeEquationSet blockXSet;

	int y_end = pY + blockSize;
	int x_end = pX + blockSize;

	for (int y = pY; y < y_end; y++)
	{
		blockXSet = blockYSet;
		for (int x = pX; x < x_end; x++)
		{
			float param0 = pInvCamZ[0] * blockXSet.e0.value;
			float param1 = pInvCamZ[1] * blockXSet.e1.value;
			float param2 = pInvCamZ[2] * blockXSet.e2.value;

			float curr_camera_z = 1 / (param0 + param1 + param2);
			float curr_ndc_z = curr_camera_z * (pTriangle.m_vertex[0].m_pos.z * param0 +
				pTriangle.m_vertex[1].m_pos.z * param1 +
				pTriangle.m_vertex[2].m_pos.z * param2);

			if (curr_ndc_z < pDepthBuffer->GetPixel(x, y))
			{
				pDepthBuffer->SetPixel(curr_ndc_z, x, y);

				Fragment curr_fragment_in;

				pFun(pTriangle.m_vertex[0], pTriangle.m_vertex[1], pTriangle.m_vertex[2],
					param0 * curr_camera_z, param1 * curr_camera_z, param2 * curr_camera_z,
					curr_fragment_in);

				pFragments.emplace_back(curr_fragment_in);
				pFragmentIndexes.emplace_back(Vec2I(x, y));
			}
			blockXSet.incrementX();
		}
		blockYSet.incrementY();
	}
}

inline void RenderIntersectBlock(const RasterizerInterpolationFun &pFun, const Triangle &pTriangle, const float(&pInvCamZ)[3], const EdgeEquationSet &pSet, const int &pArea, const int &pX, const int &pY,
	std::shared_ptr<ExtensionImage<float>> pDepthBuffer, std::vector<Fragment> &pFragments, std::vector<Vec2I> &pFragmentIndexes)
{
	EdgeEquationSet blockYSet = pSet;
	EdgeEquationSet blockXSet;

	int y_end = pY + blockSize;
	int x_end = pX + blockSize;

	for (int y = pY; y < y_end; y++)
	{
		blockXSet = blockYSet;
		for (int x = pX; x < x_end; x++)
		{
			if (blockXSet.evaluate())
			{
				float param0 = pInvCamZ[0] * blockXSet.e0.value;
				float param1 = pInvCamZ[1] * blockXSet.e1.value;
				float param2 = pInvCamZ[2] * blockXSet.e2.value;

				float curr_camera_z = 1 / (param0 + param1 + param2);
				float curr_ndc_z = curr_camera_z * (pTriangle.m_vertex[0].m_pos.z * param0 +
					pTriangle.m_vertex[1].m_pos.z * param1 +
					pTriangle.m_vertex[2].m_pos.z * param2);

				if (curr_ndc_z < pDepthBuffer->GetPixel(x, y))
				{
					pDepthBuffer->SetPixel(curr_ndc_z, x, y);

					Fragment curr_fragment_in;

					pFun(pTriangle.m_vertex[0], pTriangle.m_vertex[1], pTriangle.m_vertex[2],
						param0 * curr_camera_z, param1 * curr_camera_z, param2 * curr_camera_z,
						curr_fragment_in);

					pFragments.emplace_back(curr_fragment_in);
					pFragmentIndexes.emplace_back(Vec2I(x, y));
				}
			}
			blockXSet.incrementX();
		}
		blockYSet.incrementY();
	}
}

//线段相交测试
inline bool FasterLineSegmentIntersection(const Vec2I &p1, const Vec2I &p2, const Vec2I &p3, const Vec2I &p4) {

	Vec2I a = p2 - p1;
	Vec2I b = p3 - p4;
	Vec2I c = p1 - p3;

	int alphaDenominator = a.y*b.x - a.x*b.y;
	int betaDenominator = alphaDenominator;

	bool doIntersect = true;

	if (alphaDenominator == 0 || betaDenominator == 0) {
		doIntersect = false;
	}
	else {
		int alphaNumerator = b.y*c.x - b.x*c.y;

		if (alphaDenominator > 0) {
			if (alphaNumerator < 0 || alphaNumerator > alphaDenominator) {
				doIntersect = false;
			}
		}
		else if (alphaNumerator > 0 || alphaNumerator < alphaDenominator) {
			doIntersect = false;
		}

		int betaNumerator = a.x*c.y - a.y*c.x;

		if (doIntersect && betaDenominator > 0) {
			if (betaNumerator < 0 || betaNumerator > betaDenominator) {
				doIntersect = false;
			}
		}
		else if (betaNumerator > 0 || betaNumerator < betaDenominator) {
			doIntersect = false;
		}
	}

	return doIntersect;
}

//线段三角形相交测试
inline bool SegmentTriangleIntersection(const Vec2I &p0, const Vec2I &p1, const Vec2I &v0, const Vec2I &v1, const Vec2I &v2)
{
	if (FasterLineSegmentIntersection(p0, p1, v0, v1) ||
		FasterLineSegmentIntersection(p0, p1, v1, v2) ||
		FasterLineSegmentIntersection(p0, p1, v2, v0))
	{
		return true;
	}

	return false;
}

//block线段与三角形线段相交测试
inline bool BlockTriangleSegmentIntersection(const Vec2I &ltCorner, const int &blockWidth, const int &blockHeight, const Vec2I &v0, const Vec2I &v1, const Vec2I &v2)
{
	Vec2I rtCorner(ltCorner.x + blockWidth, ltCorner.y);
	Vec2I lbCorner(ltCorner.x, ltCorner.y + blockHeight);
	Vec2I rbCorner(ltCorner.x + blockWidth, ltCorner.y + blockHeight);

	if (SegmentTriangleIntersection(lbCorner, rbCorner, v0, v1, v2) ||
		SegmentTriangleIntersection(lbCorner, ltCorner, v0, v1, v2) ||
		SegmentTriangleIntersection(ltCorner, rtCorner, v0, v1, v2) ||
		SegmentTriangleIntersection(rtCorner, rbCorner, v0, v1, v2))
	{
		return true;
	}

	return false;
}


class Rasterizer
{
public:
	Rasterizer() 
	{
		m_inter_fun = BaseRaterFun;
	};
	~Rasterizer() {};

	void SetViewport(const Viewport &pViewport)
	{
		m_viewport = pViewport;
	}

	void SetFragmentLayout(const FragmentLayout &layout)
	{
		switch (layout)
		{
		case FragmentLayout::BASE:
			m_inter_fun = BaseRaterFun;
			break;
		case FragmentLayout::EXTENSION0:
			m_inter_fun = Extension0RaterFun;
			break;
		case FragmentLayout::EXTENSION1:
			m_inter_fun = Extension1RaterFun;
			break;
		default:
			break;
		}
	}

	void Rasterize(Triangle &pTriangle, std::vector<Fragment> &pFragments,
		std::vector<Vec2I> &pFragmentIndexes, std::shared_ptr<ExtensionImage<float>> pDepthBuffer)
	{
		float inv_camera_z[3];
		inv_camera_z[0] = 1 / pTriangle.m_vertex[0].m_pos.w;
		inv_camera_z[1] = 1 / pTriangle.m_vertex[1].m_pos.w;
		inv_camera_z[2] = 1 / pTriangle.m_vertex[2].m_pos.w;

		pTriangle.m_vertex[0].m_pos *= inv_camera_z[0];
		pTriangle.m_vertex[1].m_pos *= inv_camera_z[1];
		pTriangle.m_vertex[2].m_pos *= inv_camera_z[2];

		Vec2I raster_pos[3];
		raster_pos[0] = NDCSpaceToRasterSpace(pTriangle.m_vertex[0].m_pos, m_viewport.m_width, m_viewport.m_height);
		raster_pos[1] = NDCSpaceToRasterSpace(pTriangle.m_vertex[1].m_pos, m_viewport.m_width, m_viewport.m_height);
		raster_pos[2] = NDCSpaceToRasterSpace(pTriangle.m_vertex[2].m_pos, m_viewport.m_width, m_viewport.m_height);

		Vec2I box_min, box_max;
		box_min.x = static_cast<int>(max(min(min(raster_pos[0].x, raster_pos[1].x), raster_pos[2].x), m_viewport.m_top_leftx));
		box_min.y = static_cast<int>(max(min(min(raster_pos[0].y, raster_pos[1].y), raster_pos[2].y), m_viewport.m_top_lefty));

		box_max.x = static_cast<int>(min(max(max(raster_pos[0].x, raster_pos[1].x), raster_pos[2].x), m_viewport.m_width - 1));
		box_max.y = static_cast<int>(min(max(max(raster_pos[0].y, raster_pos[1].y), raster_pos[2].y), m_viewport.m_height - 1));

		if (box_min.x >= box_max.x || box_min.y >= box_max.y)
		{
			return;
		}

		auto notBlockSize = ~(blockSize - 1);

		box_min.x = box_min.x & notBlockSize;
		box_min.y = box_min.y & notBlockSize;

		box_max.x = box_max.x & notBlockSize;
		box_max.y = box_max.y & notBlockSize;

		Vec2I p(box_min.x, box_min.y);

		EdgeEquation tiangle_equation(raster_pos[0], raster_pos[1], raster_pos[2]);

		if (tiangle_equation.value <= 0) 
		{
			return;
		}

		EdgeEquationSet set(raster_pos[0], raster_pos[1], raster_pos[2], p);

		EdgeEquationSet setX, setY;
		setY = set;

		for (int y = box_min.y; y <= box_max.y; y += blockSize)
		{
			setX = setY;
			for (int x = box_min.x; x <= box_max.x; x += blockSize)
			{
				EdgeEquationSet temp = setX;
				EdgeEquationSet leftTopCorner = temp;
				temp.incrementX(blockSize - 1);
				EdgeEquationSet rightTopCorner = temp;
				temp.incrementY(blockSize - 1);
				EdgeEquationSet rightBottomCorner = temp;
				temp.incrementX(1 - blockSize);
				EdgeEquationSet leftBottomCorner = temp;

				bool lb = leftBottomCorner.evaluate();
				bool lt = leftTopCorner.evaluate();
				bool rb = rightBottomCorner.evaluate();
				bool rt = rightTopCorner.evaluate();
				
				setX.incrementX(blockSize);

				bool inside = lb && lt && rb && rt;

				if (inside)
				{
					RenderInsideBlock(m_inter_fun, pTriangle, inv_camera_z, leftTopCorner, tiangle_equation.value, x, y, pDepthBuffer, pFragments, pFragmentIndexes);
					continue;
				}
				
				bool intersect = lb || lt || rb || rt;

				if (!intersect)
				{
					auto pointInsideAABB = [](const Vec2I &min, const Vec2I &max, const Vec2I &point)
					{
						return (point.x >= min.x && point.x <= max.x) && (point.y >= min.y && point.y <= max.y);
					};
					
					Vec2I aabbMin(x, y);
					Vec2I aabbMax(x + blockSize - 1, y + blockSize - 1);
					if (pointInsideAABB(aabbMin, aabbMax, raster_pos[0]) ||
						pointInsideAABB(aabbMin, aabbMax, raster_pos[1]) ||
						pointInsideAABB(aabbMin, aabbMax, raster_pos[2]))
					{
						RenderIntersectBlock(m_inter_fun, pTriangle, inv_camera_z, leftTopCorner, tiangle_equation.value, x, y, pDepthBuffer, pFragments, pFragmentIndexes);
						continue;
					}
										
					if (BlockTriangleSegmentIntersection(aabbMin, blockSize - 1, blockSize - 1, 
						raster_pos[0], raster_pos[1], raster_pos[2]))
					{
						RenderIntersectBlock(m_inter_fun, pTriangle, inv_camera_z, leftTopCorner, tiangle_equation.value, x, y, pDepthBuffer, pFragments, pFragmentIndexes);
						continue;
					}
					
					continue;
				}

				RenderIntersectBlock(m_inter_fun, pTriangle, inv_camera_z, leftTopCorner, tiangle_equation.value, x, y, pDepthBuffer, pFragments, pFragmentIndexes);
			}
			setY.incrementY(blockSize);
		}
	}

private:
	Vec2I NDCSpaceToRasterSpace(const Vec4f &pPos, const float &pWidth, const float &pHeight)
	{
		Vec2I raster_pos;
		raster_pos.x = static_cast<int>((1 + pPos.x)* 0.5f * pWidth);
		raster_pos.y = static_cast<int>((1 - pPos.y)* 0.5f * pHeight);
		return raster_pos;
	}

	RasterizerInterpolationFun m_inter_fun;
	Viewport m_viewport;
};
#endif // !RASTERIZER_H
