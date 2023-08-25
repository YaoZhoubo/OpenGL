#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//设置缓存刷新时间
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	//输出版本号
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		//顶点缓冲区数据
		float positons[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f,
		};

		//索引缓冲区数据
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positons, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		//指定一个相对路径，这样的话在运行可执行文件时，默认的工作目录是包含可执行文件的目录
		//但我们通过VisualStudio调试器运行，工作目录是可以由VisualStudio设置的，项目属性->调试->工作目录
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.4f, 0.5f, 1.0f);

		Texture texture("res/textures/suanmiao.jpg");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		//取消绑定
		va.Unbind();
		vb.Unbind();
		ib.UnBind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.0f, 1.0f);

			renderer.Draw(va, ib, shader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}