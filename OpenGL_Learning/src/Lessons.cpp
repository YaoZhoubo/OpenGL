#include "Lessons.h"
#include <iostream>

void Lessons::HelloWindow()
{
	/*
	初始化和配置GLFW
	第一个参数代表选项的名称，我们可以从很多以GLFW_开头的枚举值中选择
	第二个参数接受一个整型，用来设置这个选项的值
	*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //主版本号MAJOR设置为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //次版本号MINOR设置为3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //指定模式为核心模式(Core-profile)
	// --------------------------------------------------------------

	// GLFW窗口创建
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL_Learning", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window); // 将窗口的上下文设置为当前线程的主上下文
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// 每次窗口大小被调整的时候被调用
	// --------------------------------------------------------------

	// 加载所有的OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return -1;
	}
	// --------------------------------------------------------------

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		/*
		清空屏幕的颜色缓冲
		接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
		可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
		由于现在我们只关心颜色值，所以我们只清空颜色缓冲。
		*/
		glClear(GL_COLOR_BUFFER_BIT);

		// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// --------------------------------------------------------------

	glfwTerminate();
}

void Lessons::HellowTraigle()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// 顶点数据
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f 
	};
	// 索引数据
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	/*
	定义顶点数据后，我们会把它作为输入发送给图形渲染管线的第一个处理阶段：顶点着色器。
	它会在GPU上创建内存用于储存我们的顶点数据，我们还需要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。
	顶点着色器接着会处理我们在内存中指定数量的顶点。 
	我们通过VBO管理这个内存，它会在GPU内存中储存大量顶点。
	当数据发送至显卡的内存中后，顶点着色器几乎能立即访问顶点，这是个非常快的过程。
	我们可以使用glGenBuffers函数和一个缓冲ID生成一个缓冲对象

	顶点数组对象(VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。
	这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。
	这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。刚刚设置的所有状态都将存储在VAO中
	一个顶点数组对象会储存以下这些内容：
		glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
		通过glVertexAttribPointer设置的顶点属性配置。
		通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。
	*/
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/*
	OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
	OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。
	我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上。
	一旦绑定，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
	然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
	*/
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	/*
	glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
	第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	第三个参数是我们希望发送的实际数据。
	第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
		GL_STATIC_DRAW ：数据不会或几乎不会改变。
		GL_DYNAMIC_DRAW：数据会被改变很多。
		GL_STREAM_DRAW ：数据每次绘制时都会改变。
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	顶点属性默认是禁用的，需要先启用顶点属性，以顶点属性位置值0作为参数
	glVertexAttribPointer函数用于解释顶点数据
	第一个参数指定我们要配置的顶点属性。对应我们在顶点着色器中使用layout(location = 0)定义的position顶点属性的位置值(Location)，它把顶点属性的位置值设置为0。所以这里我们传入0。
	第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec都是由浮点数值组成的)。
	第四个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
	第五个参数叫做步长(Stride)，表示每一组顶点属性之间的间隔。
	比如这里只有位置这一个顶点属性，下一组数据就是3个float之后，所以把步长设置为3 * sizeof(float)。
	我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
	一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔。
	最后一个参数为偏移量(Offset)，类型是void*，所以需要我们进行这个奇怪的强制类型转换。表示位置数据在缓冲中与起始位置的偏移量(Offset)。
	由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。

	glVertexAttribPointer从VBO管理的内存中获得它的数据，一次只会绑定一个VBO
	所以是根据调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。
	由于在调用glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性现在会链接到它的顶点数据。
	*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);// 解绑VAO供之后使用。

	/*
	顶点着色器(Vertex Shader)
	通过in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
	现在我们只关心位置(Position)数据，所以我们只需要一个顶点属性。
	每个顶点有一个3D坐标，我们就创建一个vec3变量aPos。
	gl_Position变量在幕后是vec4类型的。
	gl_Position设置的值会成为该顶点着色器的输出。
	目前我们对输入的数据什么都没有处理就把它传到着色器的输出了。
	在真实的程序里输入数据通常都不是标准化设备坐标，所以我们首先必须先把它们转换至OpenGL的可视区域内。

	片段着色器(Fragment Shader)
	用于渲染三角形的着色器。片段着色器所做的是计算像素最后的颜色输出。
	片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色，我们应该自己将其计算出来。
	声明输出变量可以使用out关键字，这里我们命名为FragColor。
	*/
	const char* vertexShaderSource = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = 
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	/*
	为了能够让OpenGL使用着色器，我们必须在运行时动态编译它的源代码。
	首先创建一个着色器对象，我们储存这个顶点着色器为unsigned int，然后用glCreateShader创建着色器，指定着色器类型为GL_VERTEX_SHADER。
	下一步用glShaderSource函数把着色器源码附加到着色器对象上。
	第一个参数为要编译的着色器对象。
	第二参数指定了传递的源码字符串数量，这里为1。
	第三个参数是顶点着色器真正的源码。
	第四个参数我们先设置为NULL。
	然后编译它
	*/
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	
	/*
	你可能会希望检测在调用glCompileShader后编译是否成功了，如果没成功的话，你还会希望知道错误是什么，这样你才能修复它们。
	首先我们定义一个整型变量来表示是否成功编译，还定义了一个储存错误消息（如果有的话）的容器。
	然后我们用glGetShaderiv检查是否编译成功。
	如果编译失败，我们会用glGetShaderInfoLog获取错误消息，然后打印它。
	*/
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/*
	着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
	如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序。
	已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
	当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入。当输出和输入不匹配的时候，你会得到一个连接错误。
	创建一个程序对象很简单：
	glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	然后把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
	得到的结果就是一个程序对象，我们可以调用glUseProgram函数，用刚创建的程序对象作为它的参数，以激活这个程序对象
	在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。
	对了，在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	*/
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检查链接着色器程序是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return;
}

void Lessons::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Lessons::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
