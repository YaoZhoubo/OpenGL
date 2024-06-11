#include "Lessons.h"
#include <iostream>

void Lessons::HelloWindow()
{
	/*
	��ʼ��������GLFW
	��һ����������ѡ������ƣ����ǿ��ԴӺܶ���GLFW_��ͷ��ö��ֵ��ѡ��
	�ڶ�����������һ�����ͣ������������ѡ���ֵ
	*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //���汾��MAJOR����Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //�ΰ汾��MINOR����Ϊ3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //ָ��ģʽΪ����ģʽ(Core-profile)
	// --------------------------------------------------------------

	// GLFW���ڴ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL_Learning", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window); // �����ڵ�����������Ϊ��ǰ�̵߳���������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// ÿ�δ��ڴ�С��������ʱ�򱻵���
	// --------------------------------------------------------------

	// �������е�OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return -1;
	}
	// --------------------------------------------------------------

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		/*
		�����Ļ����ɫ����
		����һ������λ(Buffer Bit)��ָ��Ҫ��յĻ���
		���ܵĻ���λ��GL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT
		������������ֻ������ɫֵ����������ֻ�����ɫ���塣
		*/
		glClear(GL_COLOR_BUFFER_BIT);

		// ��鲢�����¼�����������
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

	// ��������
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f 
	};
	// ��������
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	/*
	���嶥�����ݺ����ǻ������Ϊ���뷢�͸�ͼ����Ⱦ���ߵĵ�һ������׶Σ�������ɫ����
	������GPU�ϴ����ڴ����ڴ������ǵĶ������ݣ����ǻ���Ҫ����OpenGL��ν�����Щ�ڴ棬����ָ������η��͸��Կ���
	������ɫ�����Żᴦ���������ڴ���ָ�������Ķ��㡣 
	����ͨ��VBO��������ڴ棬������GPU�ڴ��д���������㡣
	�����ݷ������Կ����ڴ��к󣬶�����ɫ���������������ʶ��㣬���Ǹ��ǳ���Ĺ��̡�
	���ǿ���ʹ��glGenBuffers������һ������ID����һ���������

	�����������(VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С�
	�����ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ�
	��ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ��ո����õ�����״̬�����洢��VAO��
	һ�������������ᴢ��������Щ���ݣ�
		glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ��á�
		ͨ��glVertexAttribPointer���õĶ����������á�
		ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����
	*/
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/*
	OpenGL�кܶ໺��������ͣ����㻺�����Ļ���������GL_ARRAY_BUFFER��
	OpenGL��������ͬʱ�󶨶�����壬ֻҪ�����ǲ�ͬ�Ļ������͡�
	���ǿ���ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	һ���󶨣�����ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
	Ȼ�����ǿ��Ե���glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	*/
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	/*
	glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
	��һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
	����������������ϣ�����͵�ʵ�����ݡ�
	���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
		GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
		GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
		GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	��������Ĭ���ǽ��õģ���Ҫ�����ö������ԣ��Զ�������λ��ֵ0��Ϊ����
	glVertexAttribPointer�������ڽ��Ͷ�������
	��һ������ָ������Ҫ���õĶ������ԡ���Ӧ�����ڶ�����ɫ����ʹ��layout(location = 0)�����position�������Ե�λ��ֵ(Location)�����Ѷ������Ե�λ��ֵ����Ϊ0�������������Ǵ���0��
	�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec�����ɸ�����ֵ��ɵ�)��
	���ĸ��������������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
	�����������������(Stride)����ʾÿһ�鶥������֮��ļ����
	��������ֻ��λ����һ���������ԣ���һ�����ݾ���3��float֮�����԰Ѳ�������Ϊ3 * sizeof(float)��
	����Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
	һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ����
	���һ������Ϊƫ����(Offset)��������void*��������Ҫ���ǽ��������ֵ�ǿ������ת������ʾλ�������ڻ���������ʼλ�õ�ƫ����(Offset)��
	����λ������������Ŀ�ͷ������������0�����ǻ��ں�����ϸ�������������

	glVertexAttribPointer��VBO������ڴ��л���������ݣ�һ��ֻ���һ��VBO
	�����Ǹ��ݵ���glVertexAttribPointerʱ�󶨵�GL_ARRAY_BUFFER��VBO�����ġ�
	�����ڵ���glVertexAttribPointer֮ǰ�󶨵�����ǰ�����VBO���󣬶����������ڻ����ӵ����Ķ������ݡ�
	*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);// ���VAO��֮��ʹ�á�

	/*
	������ɫ��(Vertex Shader)
	ͨ��in�ؼ��֣��ڶ�����ɫ�����������е����붥������(Input Vertex Attribute)��
	��������ֻ����λ��(Position)���ݣ���������ֻ��Ҫһ���������ԡ�
	ÿ��������һ��3D���꣬���Ǿʹ���һ��vec3����aPos��
	gl_Position������Ļ����vec4���͵ġ�
	gl_Position���õ�ֵ���Ϊ�ö�����ɫ���������
	Ŀǰ���Ƕ����������ʲô��û�д���Ͱ���������ɫ��������ˡ�
	����ʵ�ĳ�������������ͨ�������Ǳ�׼���豸���꣬�����������ȱ����Ȱ�����ת����OpenGL�Ŀ��������ڡ�

	Ƭ����ɫ��(Fragment Shader)
	������Ⱦ�����ε���ɫ����Ƭ����ɫ���������Ǽ�������������ɫ�����
	Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ������Ӧ���Լ�������������
	���������������ʹ��out�ؼ��֣�������������ΪFragColor��
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
	Ϊ���ܹ���OpenGLʹ����ɫ�������Ǳ���������ʱ��̬��������Դ���롣
	���ȴ���һ����ɫ���������Ǵ������������ɫ��Ϊunsigned int��Ȼ����glCreateShader������ɫ����ָ����ɫ������ΪGL_VERTEX_SHADER��
	��һ����glShaderSource��������ɫ��Դ�븽�ӵ���ɫ�������ϡ�
	��һ������ΪҪ�������ɫ������
	�ڶ�����ָ���˴��ݵ�Դ���ַ�������������Ϊ1��
	�����������Ƕ�����ɫ��������Դ�롣
	���ĸ���������������ΪNULL��
	Ȼ�������
	*/
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	
	/*
	����ܻ�ϣ������ڵ���glCompileShader������Ƿ�ɹ��ˣ����û�ɹ��Ļ����㻹��ϣ��֪��������ʲô������������޸����ǡ�
	�������Ƕ���һ�����ͱ�������ʾ�Ƿ�ɹ����룬��������һ�����������Ϣ������еĻ�����������
	Ȼ��������glGetShaderiv����Ƿ����ɹ���
	�������ʧ�ܣ����ǻ���glGetShaderInfoLog��ȡ������Ϣ��Ȼ���ӡ����
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
	��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
	���Ҫʹ�øղű������ɫ�����Ǳ������������(Link)Ϊһ����ɫ���������Ȼ������Ⱦ�����ʱ�򼤻������ɫ������
	�Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ�á�
	��������ɫ����һ�������ʱ�������ÿ����ɫ����������ӵ��¸���ɫ�������롣����������벻ƥ���ʱ�����õ�һ�����Ӵ���
	����һ���������ܼ򵥣�
	glCreateProgram��������һ�����򣬲������´�����������ID���á�
	Ȼ���֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram��������
	�õ��Ľ������һ������������ǿ��Ե���glUseProgram�������øմ����ĳ��������Ϊ���Ĳ������Լ�������������
	��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������Ҳ����֮ǰд����ɫ��)�ˡ�
	���ˣ��ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ�����ˣ�
	*/
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// ���������ɫ�������Ƿ�ʧ��
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
