#pragma once
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct ShaderSourceCode;

class Shader
{
public:
	Shader(const char* filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(std::string uniformName, const glm::vec4& values);
	void SetUniform1i(std::string uniformName, int value);
	void SetUniform4Mat(std::string uniformName, glm::mat4 u_MVP);
	void SetUniform3f(std::string uniformName, const glm::vec3& values);
	void SetUniform1f(std::string uniformName, const float& value);

private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_Locations;

	unsigned int createShader(unsigned int vertex, unsigned int fragment);
	ShaderSourceCode parseShader(const char* filePath);
	unsigned int compileShader(unsigned int shaderType, const char* sourceCode);
	unsigned int GetUniformLocation(std::string uniformName);

};

