#pragma once
#include <string>
#include<unordered_map>

#include "glm/glm.hpp"
namespace cabbage {

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	private:
		std::string m_FilePath;
		unsigned int m_RendererId;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		//Set uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1iv(const std::string& name,unsigned int count, const int* value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform1fv(const std::string& name,unsigned int count, const float* value);
		void SetUniform3fv(const std::string& name, glm::vec3 value);
		void SetUniform3fv(const std::string& name, unsigned int count, glm::vec3& value);
		void SetUniform4fv(const std::string& name, glm::vec3 value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation( const std::string& name);
	};

}
