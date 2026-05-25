#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

// main shader base class
class Shader
{
public:
	Shader() = default;
	virtual ~Shader();

	void init(const std::string& vertFile, const std::string& fragFile);
	void Bind() const;

	// Setters
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;

protected:
	virtual void bindAttributes() {}
	virtual void getAllUniformLocations() {}

	GLint getUniform(const std::string& name) const;

	GLuint shaderID = 0;

private:
	std::string loadShader(const std::string& fileName) ;
	GLuint compileShader(const std::string& source, unsigned int type);
	void checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

};