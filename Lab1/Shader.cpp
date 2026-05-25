#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::~Shader()
{
    glDeleteProgram(shaderID);
}

void Shader::init(const std::string& vertFile, const std::string& fragFile)
{
    shaderID = glCreateProgram();

    std::string vertSrc = loadShader(vertFile);
    std::string fragSrc = loadShader(fragFile);

    GLuint vert = compileShader(vertSrc, GL_VERTEX_SHADER);
    GLuint frag = compileShader(fragSrc, GL_FRAGMENT_SHADER);

    glAttachShader(shaderID, vert);
    glAttachShader(shaderID, frag);

    bindAttributes();

    glLinkProgram(shaderID);
    checkError(shaderID, GL_LINK_STATUS, true, "Error linking shader program");

    GLint success = 0;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);

    if (!success)
    {
        char log[2048];
        glGetProgramInfoLog(shaderID, 2048, nullptr, log);
        std::cerr << "PROGRAM LINK ERROR:\n" << log << std::endl;
    }


    glValidateProgram(shaderID);
    checkError(shaderID, GL_VALIDATE_STATUS, true, "Error validating shader program");

    glDeleteShader(vert);
    glDeleteShader(frag);

    getAllUniformLocations();
}

void Shader::Bind() const
{
    glUseProgram(shaderID);
}

std::string Shader::loadShader(const std::string& fileName)
{
    std::cout << "Loading shader: " << fileName << std::endl;

    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Failed to open shader file: " << fileName << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint Shader::compileShader(const std::string& source, unsigned int type)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[2048];
        glGetShaderInfoLog(shader, 2048, nullptr, log);
        std::cerr << "SHADER COMPILE ERROR:\n" << log << std::endl;
    }

    checkError(shader, GL_COMPILE_STATUS, false, "Error compiling shader");
    return shader;
}


void Shader::checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024];

    if (isProgram)
    {
        glGetProgramiv(shader, flag, &success);
    }
    else
    {
        glGetShaderiv(shader, flag, &success);
    }

    if (!success)
    {
        if (isProgram)
        {
            glGetProgramInfoLog(shader, sizeof(error), nullptr, error);
        }
        else
        {
            glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
        }

        std::cerr << errorMessage << ": " << error << std::endl;
    }
}

GLint Shader::getUniform(const std::string& name) const
{
    return glGetUniformLocation(shaderID, name.c_str());
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(getUniform(name), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(getUniform(name), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
    glUniform3f(getUniform(name), vec.x, vec.y, vec.z);
}
