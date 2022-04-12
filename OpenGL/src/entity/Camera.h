#pragma once

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

class Frustum;

class Camera
{
private:
	glm::vec3 m_Position, m_Front, m_Up, m_Right;
	glm::mat4 m_View, m_Proj;

	double m_Yaw, m_Pitch;
	float m_CamMoveSpeed, m_CamRotSpeed;

	float m_Fov, m_Aspect, m_ZNear, m_ZFar;

	std::unique_ptr<Frustum> m_Frustum;

	void setProjectionMatrix();
	void updateCameraView();

public:
	Camera(float fov, int width, int height, float nearZ, float farZ);
	~Camera();

	void Update(float dt);

	inline float GetFov() const { return m_Fov; }
	inline float GetAspect() const { return m_Aspect; }
	inline float GetZNear() const { return m_ZNear; }
	inline float GetZFar() const { return m_ZFar; }
	inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
	inline glm::mat4 GetProjMatrix() const { return m_Proj; }
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::vec3 GetRotation() const { return m_Front; }
	inline glm::vec3 GetUp() const { return m_Up; }
	inline glm::vec3 GetRight() const { return m_Right; }
	inline Frustum* GetFrustum() const { return m_Frustum.get(); }
};
