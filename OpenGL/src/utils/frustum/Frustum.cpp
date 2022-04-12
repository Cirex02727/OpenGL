#include "Frustum.h"

Frustum::Frustum() : m_Camera(nullptr) {}

Frustum::Frustum(Camera* camera)
{
    m_Camera = camera;
    update();
}

Frustum::~Frustum()
{
}

void Frustum::update()
{
    const float halfVSide = m_Camera->GetZFar() * tanf(m_Camera->GetFov() * 0.5f);
    const float halfHSide = halfVSide * m_Camera->GetAspect();
    const glm::vec3 frontMultFar = m_Camera->GetZFar() * m_Camera->GetRotation();

    glm::mat4 matrix = m_Camera->GetViewMatrix() * m_Camera->GetProjMatrix();

    m_NearFace = { m_Camera->GetPosition() + m_Camera->GetZNear() * m_Camera->GetRotation(), m_Camera->GetRotation() };
    m_FarFace = { m_Camera->GetPosition() + frontMultFar, -m_Camera->GetRotation() };
    m_RightFace = { m_Camera->GetPosition(), glm::cross(m_Camera->GetUp(),frontMultFar + m_Camera->GetRight() * halfHSide) };
    m_LeftFace = { m_Camera->GetPosition(), glm::cross(frontMultFar - m_Camera->GetRight() * halfHSide, m_Camera->GetUp()) };
    m_TopFace = { m_Camera->GetPosition(), glm::cross(m_Camera->GetRight(), frontMultFar - m_Camera->GetUp() * halfVSide) };
    m_BottomFace = { m_Camera->GetPosition(), glm::cross(frontMultFar + m_Camera->GetUp() * halfVSide, m_Camera->GetRight()) };
}

bool Frustum::isOnFrustum(const AABB& aabb, const glm::mat4& model) const
{
    //Get global scale thanks to our transform
    const glm::vec3 globalCenter{ model * glm::vec4(aabb.m_Center, 1.f) };

    // Scaled orientation
    const glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f) * aabb.m_Extents.x;
    const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f) * aabb.m_Extents.y;
    const glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f) * aabb.m_Extents.z;

    const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

    const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

    const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

    //We not need to divise scale because it's based on the half extention of the AABB
    const AABB globalAABB(globalCenter, glm::vec3(newIi, newIj, newIk));

    return isOnOrForwardPlan(globalAABB, m_LeftFace) &&
        isOnOrForwardPlan(globalAABB, m_RightFace) &&
        isOnOrForwardPlan(globalAABB, m_TopFace) &&
        isOnOrForwardPlan(globalAABB, m_BottomFace) &&
        isOnOrForwardPlan(globalAABB, m_NearFace) &&
        isOnOrForwardPlan(globalAABB, m_FarFace);
}

bool Frustum::isChunkOnFrustum(const AABB& aabb) const
{
    return isOnOrForwardPlan(aabb, m_LeftFace) &&
        isOnOrForwardPlan(aabb, m_RightFace) &&
        isOnOrForwardPlan(aabb, m_TopFace) &&
        isOnOrForwardPlan(aabb, m_BottomFace) &&
        isOnOrForwardPlan(aabb, m_NearFace) &&
        isOnOrForwardPlan(aabb, m_FarFace);
}

bool Frustum::isOnOrForwardPlan(const AABB& aabb, const Plane& plan) const
{
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    const float r = aabb.m_Extents.x * std::abs(plan.m_Normal.x) +
        aabb.m_Extents.y * std::abs(plan.m_Normal.y) + aabb.m_Extents.z * std::abs(plan.m_Normal.z);

    return -r <= plan.getSignedDistanceToPlan(aabb.m_Center);
}
