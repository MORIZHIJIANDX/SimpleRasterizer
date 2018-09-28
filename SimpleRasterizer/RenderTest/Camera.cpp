#include "Camera.h"

Camera::Camera() : m_pos(0, 0, 0), m_right(1, 0, 0), m_look(0, 0, 1), m_up(0, 1, 0)
{
	SetLens(AngleToRadian(90.0f), 1.0f, 1.0f, 1000.0f);
	UpdateViewMatrix();
}

Camera::~Camera()
{

}

Vec3f Camera::GetPosition() const
{
	return m_pos;
}

Vec3f Camera::GetRight() const
{
	return m_right;
}

Vec3f Camera::GetLook() const
{
	return m_look;
}

Vec3f Camera::GetUp() const
{
	return m_up;
}

float Camera::GetNearZ() const
{
	return m_nearz;
}

float Camera::GetFarZ() const
{
	return m_farz;
}

float Camera::GetAspect() const
{
	return m_aspect;
}

float Camera::GetFovY() const
{
	return m_fovy;
}

void Camera::SetPosition(const float &pX, const float &pY, const float &pZ)
{
	m_pos.x = pX;
	m_pos.y = pY;
	m_pos.z = pZ;
}

void Camera::SetPosition(const Vec3<float> pPos)
{
	m_pos = pPos;
}

void Camera::SetLens(const float &pFovY, const float &pAspect,
	const float &pNearZ, const float &pFarZ)
{
	m_fovy = pFovY;
	m_aspect = pAspect;
	m_nearz = pNearZ;
	m_farz = pFarZ;

	m_proj = Matrix4x4PerspectiveFov<float>(m_fovy, m_aspect, m_nearz, m_farz);
}

Matrix4x4<float> Camera::GetViewMatrix() const
{
	return m_view;
}

Matrix4x4<float> Camera::GetProjMatrix() const
{
	return m_proj;
}

Matrix4x4<float> Camera::GetViewProjMatrix() const
{
	return m_view * m_proj;
}

void Camera::Walk(const float &pD)
{
	m_pos += m_look * pD;
}

void Camera::Strafe(const float &pD)
{
	m_pos += m_right * pD;
}

void Camera::Pitch(const float &pAngle)
{
	Matrix4x4<float> rotation = Matrix4x4RotationAxis<float>(LoadVector3(m_right), pAngle);

	m_up = Vec3TransformNormal(m_up, rotation);
	m_look = Vec3TransformNormal(m_look, rotation);
}

void Camera::Yaw(const float &pAngle)
{
	Matrix4x4<float> rotation = Matrix4x4RotationAxis<float>(LoadVector3(m_up), pAngle);

	m_right = Vec3TransformNormal(m_right, rotation);
	m_look = Vec3TransformNormal(m_look, rotation);
}

void Camera::UpdateViewMatrix()
{
	m_look.Normalize();
	m_up = Normalize(Cross(m_look, m_right));
	m_right = Normalize(Cross(m_up, m_look));

	m_view = ZeroMatrix4x4<float>();

	m_view[0][0] = m_right.x;
	m_view[1][0] = m_right.y;
	m_view[2][0] = m_right.z;
	m_view[3][0] = -Dot(m_pos, m_right);

	m_view[0][1] = m_up.x;
	m_view[1][1] = m_up.y;
	m_view[2][1] = m_up.z;
	m_view[3][1] = -Dot(m_pos, m_up);

	m_view[0][2] = m_look.x;
	m_view[1][2] = m_look.y;
	m_view[2][2] = m_look.z;
	m_view[3][2] = -Dot(m_pos, m_look);

	m_view[3][3] = 1;
}

void Camera::OnResize()
{
	SetLens(AngleToRadian(90.0f), 1.0f, 1.0f, 1000.0f);
}