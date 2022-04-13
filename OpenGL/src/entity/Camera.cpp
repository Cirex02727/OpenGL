#include "Camera.h"

#include <iostream>

#include "static/Input.h"
#include "static/Cursor.h"

#include "utils/frustum/Frustum.h"

Camera::Camera(float fov, int width, int height, float nearZ, float farZ)
	: m_Fov(fov), m_Aspect((float)width / (float)height), m_ZNear(nearZ), m_ZFar(farZ),
    m_Position(0.0f, 100.0f, 0.0f), m_Front(0.0f, 0.0f, 1.0f), m_Up(0.0f, 1.0f, 0.0f),
    m_CamMoveSpeed(0.01f), m_CamRotSpeed(0.1f), m_Yaw(90.0f), m_Pitch(0.0f),
    m_View(glm::mat4(1.0f))
{
    setProjectionMatrix();
    updateCameraView();

    m_Frustum = std::make_unique<Frustum>(this);
}

Camera::~Camera()
{
    if (m_Frustum)
        m_Frustum.reset();
}

void Camera::Update(float dt)
{
    float speed = m_CamMoveSpeed;
    if (Input::IsKey(GLFW_KEY_LEFT_CONTROL) || Input::IsKey(GLFW_KEY_RIGHT_CONTROL))
        speed *= 5.0f;

    if (Input::IsKey(GLFW_KEY_W))
        m_Position += m_Front * speed * dt;
    if (Input::IsKey(GLFW_KEY_S))
        m_Position -= m_Front * speed * dt;
    if (Input::IsKey(GLFW_KEY_A))
        m_Position -= m_Right * speed * dt;
    if (Input::IsKey(GLFW_KEY_D))
        m_Position += m_Right * speed * dt;

    if (Input::IsKey(GLFW_KEY_SPACE))
        m_Position += glm::vec3(0.0f, 1.0f, 0.0f) * speed * dt;
    if (Input::IsKey(GLFW_KEY_LEFT_SHIFT))
        m_Position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed * dt;

    m_Yaw += Input::GetDX() * m_CamRotSpeed;
    m_Pitch += Input::GetDY() * m_CamRotSpeed;

    if (m_Pitch > 89.5f)
        m_Pitch = 89.5f;
    if (m_Pitch < -89.5f)
        m_Pitch = -89.5f;

    m_Yaw = fmod(m_Yaw, 360.0);
    
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraView();
    m_Frustum->update();
}

void Camera::setProjectionMatrix()
{
    m_Proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_ZNear, m_ZFar);
}

void Camera::updateCameraView()
{
    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
    front.y = static_cast<float>(sin(glm::radians(m_Pitch)));
    front.z = static_cast<float>(sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
    m_Front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
