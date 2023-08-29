#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
		    -5.0f, -5.0f, 0.0f, 0.0f,
			 5.0f, -5.0f, 1.0f, 0.0f,
			 5.0f,  5.0f, 1.0f, 1.0f,
			-5.0f,  5.0f, 0.0f, 1.0f
		};

		//索引缓冲区数据
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexBuffer vb(positons, 4 * 4 * sizeof(float));
		VertexArray va;

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.f / 480.f, 0.1f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -50.0f));
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f),glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

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

		/* 设置ImGui */
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		float increment = 0.05f;
		glm::vec3 tanslation1(-10.0f, 0.0f, 0.0f);
		float r1 = 0.0f;
		glm::vec3 tanslation2(10.0f, 0.0f, 0.0f);
		float r2 = 0.0f;
		glm::vec3 tanslation3(20.0f, 0.0f, 0.0f);
		float r3 = 0.0f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();


			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), tanslation1);
				glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(r1), glm::vec3(1.0f, 0.0f, 0.0f));
				glm::mat4 mvp = projection * view * rot * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), tanslation2);
				glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(r2), glm::vec3(1.0f, 0.0f, 0.0f));
				glm::mat4 mvp = projection * view * rot * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), tanslation3);
				glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(r3), glm::vec3(1.0f, 0.0f, 0.0f));
				glm::mat4 mvp = projection * view * rot * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}


			{
				ImGui::SliderFloat3("Translation A", &tanslation1.x, -100.0f, 100.0f);
				ImGui::SliderFloat("Rotation A", &r1, -100.0f, 100.0f);
				ImGui::SliderFloat3("Translation B", &tanslation2.x, -100.0f, 100.0f);
				ImGui::SliderFloat("Rotation A", &r2, -100.0f, 100.0f);
				ImGui::SliderFloat3("Translation C", &tanslation3.x, -100.0f, 100.0f);
				ImGui::SliderFloat("Rotation A", &r3, -100.0f, 100.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}