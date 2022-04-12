#include "Input.h"

#include <iostream>
#include "imgui/imgui_impl_glfw.h"

int Input::m_Keys[GLFW_KEY_LAST];
int Input::m_Mouses[GLFW_MOUSE_BUTTON_LAST];
double Input::m_PrevMouseX = -1, Input::m_PrevMouseY = -1;
double Input::m_dX = 0, Input::m_dY = 0;
bool Input::m_FirstMouse = true;

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::m_Keys[key] = action;
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

}
void Input::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	Input::m_Mouses[button] = action;
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void Input::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	float mouseX = static_cast<float>(xpos);
	float mouseY = static_cast<float>(ypos);

	if (Input::m_FirstMouse)
	{
		Input::m_PrevMouseX = mouseX;
		Input::m_PrevMouseY = mouseY;
		Input::m_FirstMouse = false;
	}

	Input::m_dX = mouseX - Input::m_PrevMouseX;
	Input::m_dY = Input::m_PrevMouseY - mouseY;

	Input::m_PrevMouseX = mouseX;
	Input::m_PrevMouseY = mouseY;
}

void Input::Update(GLFWwindow* window)
{
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	Input::cursorCallback(window, mouseX, mouseY);
}

int Input::GetKey(int key)
{
	return Input::m_Keys[key];
}

int Input::GetMouse(int key)
{
	return Input::m_Mouses[key];
}

bool Input::IsKeyDown(int key)
{
	return Input::m_Keys[key] == GLFW_PRESS;
}

bool Input::IsKey(int key)
{
	return Input::m_Keys[key] == GLFW_PRESS || Input::m_Keys[key] == GLFW_REPEAT;
}

double Input::GetCursorX()
{
	return Input::m_PrevMouseX;
}

double Input::GetCursorY()
{
	return Input::m_PrevMouseY;
}

double Input::GetDX()
{
	return Input::m_dX;
}

double Input::GetDY()
{
	return Input::m_dY;
}
