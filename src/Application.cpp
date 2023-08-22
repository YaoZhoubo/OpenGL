#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

//用于多返回值，ParseShader()函数
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	//将目标文件从硬盘读取到内存，文件读写操作std::ofstream(写)和std::ifstream(读)属于fstream类
	std::ifstream stream(filepath);

	//准备一个string用来接收stream
	std::string line;

	//枚举着色器类型
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	//type用来匹配我们正在读取的当前着色器类型，默认为NONE
	ShaderType type = ShaderType::NONE;

	//创建一个字符串数组（2个大小）来保存顶点着色器和存片段着色器
	std::stringstream ss[2];
	
	//一行一行的读取stream，用line接收
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			//把着色器代码一行一行添加进ss中,ss[0]存VERTEX,ss[1]存FRAGMENT
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	//string不能直接转换成const char*, string是另外一个类只是其中包含const char*，所以要这么做
	const char* src = source.c_str();
	//着色器， 指定的着色器个数， 一个指向实际指针的指针
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	// TODO: Error handing
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile" << 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;

		std::cout << (const int*)message << std::endl;

		glDeleteShader(id);
		
		return 0;
	}

	return id;
}
//我们要把着色器以字符串的模式写好然后传递进来
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//这个函数返回一个无符号整数，
	unsigned int program =  glCreateProgram();

	//创建着色器
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//将这两个着色器附加到我们的程序中
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	//链接程序
	glLinkProgram(program);
	//验证程序
	glValidateProgram(program);
	//删除着色器
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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
		-0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		 0.5f, -0.5f,
	};

	//索引缓冲区数据
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//绑定顶点数组
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//绑定顶点缓冲区
	VertexBuffer vb(positons, 4 * 2 * sizeof(float));

	//激活顶点属性
	glEnableVertexAttribArray(0);
	//index 我们只有位置这一个属性，所以只要调用这个函数一次，指定一个属性索引，0，
	//size 指的是组件数目，比如这里2个浮点数表示一个顶点，就填2
	//type 指用的数据类型，这里是浮点数
	//stride 指每个顶点之间的字节数目，这里步幅就是两个浮点数的大小8, 也可以写成sizeof(float) * 2
	//		当顶点不仅仅位置属性的时候，比如还有坐标、法线等，我们可以用一个结构体去组成一个单一顶点，
	//*pointer 这里只有一个属性，所以填0， 假如有另一个属性，这里可以填(const void*)8
	//			当用结构体去定义顶点的时候，我们可以用宏的偏移量来计算这是什么，而不是写个8
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//绑定索引缓冲区
	IndexBuffer ib(indices, 6);

	//指定一个相对路径，这样的话在运行可执行文件时，默认的工作目录是包含可执行文件的目录
	//但我们通过VisualStudio调试器运行，工作目录是可以由VisualStudio设置的，项目属性->调试->工作目录
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	//这两行为调试代码，输出着色器字符串源码
	//std::cout << source.VertexSource << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	//创建着色器并使用
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	//找到统一变量u_Color的索引
	int location = glGetUniformLocation(shader, "u_Color");
	//如果location为-1，意味着没找到uniform变量
	ASSERT(location != -1);
	
	//取消绑定
	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//1. mode 指定模式，就是要画个什么东西，这里指定一个三角形
		//2. first 想要开始的下标
		//3. count 要渲染的索引的数量
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		float rr = glfwGetTime();

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, sin(rr), 0.5, 0.4, 10.0));

		/*glBindBuffer(GL_ARRAY_BUFFER, buffer);*/
		GLCall(glBindVertexArray(vao));
		ib.Bind();


		// DrawCall指令
		// 这里的第三个参数type指的是索引数据的类型,必须是GL_UNSIGNED_BYTE、GL_UNSIGNED_SHORT或GL_UNSIGNED_INT之一。
		// 最后一个参数指定当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的数据缓冲区数组的第一个索引的偏移量。
		// 通常我们绑定了buffer后，这里写成NULL就行,
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	}
	glfwTerminate();
	return 0;
}