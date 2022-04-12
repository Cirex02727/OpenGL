#pragma once

#include "GLFW/glfw3.h"

class Input
{
public:
	static int m_Keys[GLFW_KEY_LAST];
	static int m_Mouses[GLFW_MOUSE_BUTTON_LAST];
	static double m_PrevMouseX, m_PrevMouseY;
	static double m_dX, m_dY;
	static bool m_FirstMouse;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorCallback(GLFWwindow* window, double xpos, double ypos);

	static void Update(GLFWwindow* window);

	static int GetKey(int key);
	static int GetMouse(int key);

	static bool IsKeyDown(int key);
	static bool IsKey(int key);

	static double GetCursorX();
	static double GetCursorY();

	static double GetDX();
	static double GetDY();
};