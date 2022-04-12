#pragma once

#include <GLFW/glfw3.h>

class Cursor
{
private:
	static GLFWwindow* m_Window;
	static int m_HalfWidth, m_HalfHeight;

public:
	enum class CursorMode { NORMAL, DISABLED, HIDDEN };

	static void setWindow(GLFWwindow* window, int width, int height);

	static void SetCursorMode(Cursor::CursorMode mode);

	inline static unsigned int ToGLFW(const Cursor::CursorMode mode)
	{
		switch (mode)
		{
		case CursorMode::NORMAL: return GLFW_CURSOR_NORMAL;
		case CursorMode::DISABLED: return GLFW_CURSOR_DISABLED;
		case CursorMode::HIDDEN: return GLFW_CURSOR_HIDDEN;
		}

		return -1;
	}

	inline static CursorMode GetCurrentMode() { return m_CurrentMode; }

private:
	static CursorMode m_CurrentMode;
};
