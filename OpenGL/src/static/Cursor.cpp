#include "Cursor.h"

#include "static/Input.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"

GLFWwindow* Cursor::m_Window;
int Cursor::m_HalfWidth, Cursor::m_HalfHeight;
Cursor::CursorMode Cursor::m_CurrentMode;

void Cursor::setWindow(GLFWwindow* window, int width, int height)
{
	Cursor::m_Window = window;
	Cursor::m_HalfWidth = width / 2;
	Cursor::m_HalfHeight = height / 2;
}

void Cursor::SetCursorMode(Cursor::CursorMode mode)
{
	Input::m_FirstMouse = true;
	glfwSetInputMode(Cursor::m_Window, GLFW_CURSOR, Cursor::ToGLFW(mode));
	Cursor::m_CurrentMode = mode;
}
