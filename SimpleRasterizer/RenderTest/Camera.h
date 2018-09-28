#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "RenderMath.h"

class Camera
{
public:
	Camera();

	~Camera();

	Vec3f GetPosition() const;
	Vec3f GetRight() const;
	Vec3f GetLook() const;
	Vec3f GetUp() const;

	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;

	Matrix4x4<float> GetViewMatrix() const;
	Matrix4x4<float> GetProjMatrix() const;
	Matrix4x4<float> GetViewProjMatrix() const;

	void SetPosition(const float &pX, const float &pY, const float &pZ);
	void SetPosition(const Vec3<float> pPos);

	void SetLens(const float &pFovY, const float &pAspect,
		const float &pNearZ, const float &pFarZ);

	void Walk(const float &pD);

	void Strafe(const float &pD);

	void Pitch(const float &pAngle);

	void Yaw(const float &pAngle);

	void UpdateViewMatrix();

	void OnResize();

private:
	Vec3<float> m_pos;
	Vec3<float> m_right;
	Vec3<float> m_look;
	Vec3<float> m_up;

	float m_nearz;
	float m_farz;
	float m_aspect;
	float m_fovy;

	Matrix4x4<float> m_view;
	Matrix4x4<float> m_proj;
};

#endif // !CAMERA_H
