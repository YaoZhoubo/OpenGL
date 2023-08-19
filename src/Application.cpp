#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

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
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile" << 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;

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

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	//创建了一个数组来储存数据
	float positons[6] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f,
	};

	// 第一个参数是指定想要多少个缓冲区，OpenGL可以一次生成一堆缓冲区，
	// 第二个参数需要一个无符号整型指针，因为这个函数返回void，不会返回生成的缓冲区id，所以我们给它提供一个整型指针，函数就会把id写入这个指针指向的内存
	unsigned buffer;
	glGenBuffers(1, &buffer);
	// 第一个参数是目标，对我们来说这只是一个内存缓冲区，这里是一个数组，
	// 第二个参数是创建好的buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// 第一个参数同上
	// 第二个参数需要指定大小，可以直接写成sizeof(positions)，但这里的写法更清晰
	// 第三个参数指定缓冲区
	// 第四个参数根据文档，STATIC表示创建一次，多次使用（每一帧都要画），DYNAMIC表示数据储存的内容会被反复修改和多次使用
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positons, GL_STATIC_DRAW);

	//相当于激活顶点属性的使用
	glEnableVertexAttribArray(0);
	//index 我们只有位置这一个属性，所以只要调用这个函数一次，指定一个属性索引，0，
	//size 指的是组件数目，比如这里2个浮点数表示一个顶点，就填2
	//type 指用的数据类型，这里是浮点数
	//stride 指每个顶点之间的字节数目，这里步幅就是两个浮点数的大小8, 也可以写成sizeof(float) * 2
	//		当顶点不仅仅位置属性的时候，比如还有坐标、法线等，我们可以用一个结构体去组成一个单一顶点，
	//*pointer 这里只有一个属性，所以填0， 假如有另一个属性，这里可以填(const void*)8
	//			当用结构体去定义顶点的时候，我们可以用宏的偏移量来计算这是什么，而不是写个8
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


	//指定一个相对路径，这样的话在运行可执行文件时，默认的工作目录是包含可执行文件的目录
	//但我们通过VisualStudio调试器运行，工作目录是可以由VisualStudio设置的，项目属性->调试->工作目录
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	std::cout << source.VertexSource << std::endl;
	std::cout << source.FragmentSource << std::endl;

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//1. mode 指定模式，就是要画个什么东西，这里指定一个三角形
		//2. first 想要开始的下标
		//3. count 要渲染的索引的数量
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 另外一种DrawCall指令，这里的第三个参数type指的是索引数据的类型，
		// 最后一个参数通常不用，写成NULL就行
		//glDrawElements(GL_TRIANGLES, 3, ，);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}