#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<unordered_map>
#include <Windows.h>


#include "Renderer.h"
#include "log/Log.h"



Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererId(0) 
{
	ShaderProgramSource source = ParseShader(m_FilePath);
	m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
};

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererId));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
};

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
	}

	return id;
};

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	char buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);

	std::string currentDirectory(buffer);
	std::string filePath1 = currentDirectory + "\\" + filepath;

	std::ifstream stream(filepath);

	if(!stream.is_open())
	{
	LOG(Info) << "Could not open: " << filePath1.c_str() << ENDL;
	}

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
			continue;
		}
		if (type == ShaderType::NONE) continue;
		ss[(int)type] << line << '\n';
	}
	return { ss[0].str(), ss[1].str() };
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));

}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));

}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1fv(const std::string& name, unsigned int count,const float* values)
{
	GLCall(glUniform1fv(GetUniformLocation(name), count, values));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform3fv(const std::string& name, glm::vec3 value)
{
	GLCall(glUniform3fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetUniform3fv(const std::string& name, unsigned int count, glm::vec3& value)
{
	GLCall(glUniform3fv(GetUniformLocation(name), count, &value[0]));
}

void Shader::SetUniform4fv(const std::string& name, glm::vec3 value)
{
	GLCall(glUniform4fv(GetUniformLocation(name), 1, &value[0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache.at(name);

	GLCall(int location = glGetUniformLocation(m_RendererId, name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}