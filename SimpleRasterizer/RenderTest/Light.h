#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "PCH.h"
#include "RenderMath.h"

struct Material
{
	Vec4f m_ambient;
	Vec4f m_diffuse;
	Vec4f m_specular;
	Vec4f m_reflect;
};

struct DirectionalLight
{
	Vec4f m_ambient;
	Vec4f m_diffuse;
	Vec4f m_specular;
	Vec3f m_direction;
};

void ComputeDirectionalLight(const Material &pMat, const DirectionalLight &pLight, const Vec3f &pNormal,
	const Vec3f &pToEye, Vec4f &pAmbient, Vec4f &pDiffuse, Vec4f &pSpec)
{
	Vec3f light_dir = pLight.m_direction * - 1.0f;

	pAmbient = Mul(pMat.m_ambient, pLight.m_ambient);

	float diffuse_factor = Dot(light_dir, pNormal);

	if (diffuse_factor > 0.0f)
	{
		Vec3f v = VectorReflect(pLight.m_direction, pNormal);
		
		float spec_factor = std::pow(max(Dot(v, pToEye), 0.0f), pMat.m_specular.w);

		pDiffuse = Mul(pMat.m_diffuse, pLight.m_diffuse) * diffuse_factor;
		pSpec = Mul(pMat.m_specular, pLight.m_specular) * spec_factor;
	}
}

#endif // !LIGHT_H
