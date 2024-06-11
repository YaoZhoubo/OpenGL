#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Lessons
{
public:
	void HelloWindow();
	void HellowTraigle();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);

private:
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
};