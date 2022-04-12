#pragma once

#include "entity/Camera.h"

struct Plane
{
	glm::vec3 m_Normal = { 0.0f, 0.0f, 0.0f };
	float m_Distance = 0.0f;

	Plane() {}
	Plane(const glm::vec3& p1, const glm::vec3& norm)
		: m_Normal(glm::normalize(norm)), m_Distance(glm::dot(m_Normal, p1)) {}

	float getSignedDistanceToPlan(const glm::vec3& point) const
	{
		return glm::dot(m_Normal, point) - m_Distance;
	}
};

struct AABB
{
	glm::vec3 m_Center{ 0.0f, 0.0f, 0.0f }, m_Extents{ 0.0f, 0.0f, 0.0f };

	AABB() {}
	AABB(const glm::vec3& center, const glm::vec3& extends)
		: m_Center(center), m_Extents(extends) {}
};

class Frustum
{
private:
	Plane m_TopFace, m_BottomFace, m_RightFace, m_LeftFace, m_FarFace, m_NearFace;

	Camera* m_Camera;

public:
	Frustum();
	Frustum(Camera* camera);
	~Frustum();

	void update();

	bool isOnFrustum(const AABB& aabb, const glm::mat4& model) const;
	bool isChunkOnFrustum(const AABB& aabb) const;
	bool isOnOrForwardPlan(const AABB& aabb, const Plane& plan) const;
};
