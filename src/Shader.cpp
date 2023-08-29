#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"



Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);//res/shaders/Basic.shader
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;

		std::cout << (const int*)message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}
//我们要把着色器以字符串的模式写好然后传递进来
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//这个函数返回一个无符号整数，
	unsigned int program = glCreateProgram();

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

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}
