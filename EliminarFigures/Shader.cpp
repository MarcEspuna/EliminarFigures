#include "Shader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderSourceCode {
    std::string VertexShaderSource;
    std::string FragmentShaderSource;
};


Shader::Shader(const char* filePath)
{
    ShaderSourceCode sourceCode = parseShader(filePath);
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, &sourceCode.VertexShaderSource[0]);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, &sourceCode.FragmentShaderSource[0]);
    m_RendererID = createShader(vertex, fragment);
}


Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}




ShaderSourceCode Shader::parseShader(const char* filePath)
{
    enum class ShaderParser
    {
        NOT_SET = -1,
        VERTEX_SHADER = 0,
        FRAGMENT_SHADER = 1
    };

    ShaderParser shaderParser;
    shaderParser = ShaderParser::NOT_SET;

    std::string line;
    std::ifstream shaderFile(filePath);

    std::stringstream shaderSource[2];

    if (shaderFile.is_open())
    {
        std::cout << "[SHADER]: Shader file opened" << std::endl;
        while (getline(shaderFile, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    shaderParser = ShaderParser::VERTEX_SHADER;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    shaderParser = ShaderParser::FRAGMENT_SHADER;
                }
            }
            else
            {
                shaderSource[(size_t)shaderParser] << line;
                shaderSource[(size_t)shaderParser] << '\n';
            }
        }
    }
    else
    {
        std::cout << "Could not open Shader source file!" << std::endl;
    }

    return { shaderSource[0].str(), shaderSource[1].str() };
}

 unsigned int Shader::compileShader(unsigned int shaderType, const char* sourceCode)
{
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCode, 0);
    glCompileShader(shader);

    int compilation = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation);

    if (compilation == GL_FALSE)
    {
        int errorMessageLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorMessageLength);
        char* errorMessage = new char[errorMessageLength];
        glGetShaderInfoLog(shader, errorMessageLength, &errorMessageLength, errorMessage);

        glDeleteShader(shader);

        std::cout << "[SHADER COMILATION ERROR]: " << errorMessage << std::endl;
        return 0;
    }
    if (shaderType == 35633)
        std::cout << "[SHADER]: Vertex Shader succesfully compiled" << std::endl;
    else std::cout << "[SHADER]: Fragment Shader succesfully compiled" << std::endl;
    return shader;
}

 unsigned int Shader::GetUniformLocation(std::string uniformName)
 {
     int location = 0;
     if (m_Locations.find(uniformName) == m_Locations.end())
     {
         location = glGetUniformLocation(m_RendererID, &uniformName[0]);
         m_Locations[uniformName] = location;
     }
     else {
         location = m_Locations[uniformName];
     }

     if (location == -1)
     {
         //std::cout << "Uniform " << uniformName << " not found!" << std::endl;
     }

     return location;
 }

void Shader::SetUniform4f(std::string uniformName, const glm::vec4& values)
{
    Bind();
    glUniform4f(GetUniformLocation(uniformName), values.x, values.y, values.z, values.t);
}

void Shader::SetUniform3f(std::string uniformName, const glm::vec3& values)
{
    Bind();
    glUniform3f(GetUniformLocation(uniformName), values.x, values.y, values.z);
}

void Shader::SetUniform1f(std::string uniformName, const float& value)
{
    Bind();
    glUniform1f(GetUniformLocation(uniformName), value);
}

void Shader::SetUniform1i(std::string uniformName, int value)
{
    Bind();
    glUniform1i(GetUniformLocation(uniformName), value);

}

void Shader::SetUniform4Mat(std::string uniformName, glm::mat4 u_MVP)
{
    Bind();
    glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, (float*)&u_MVP[0]);

}


unsigned int Shader::createShader(unsigned int vertex, unsigned int fragment)
{
    unsigned int program;
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);
    int Linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &Linked);
    if (Linked == GL_FALSE)
    {
        int errorMessageLength = 0;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &errorMessageLength);
        char* errorMessage = new char[errorMessageLength];
        glGetShaderInfoLog(program, errorMessageLength, &errorMessageLength, errorMessage);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(program);

        std::cout << "[SHADER LINKAGE ERROR]: " << errorMessage << std::endl;
        return 0;
    }

    std::cout << "[SHADER]: Successfull linkage" << std::endl;
    return program;
}