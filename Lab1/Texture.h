#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture();

	void Bind(unsigned int unit); 
	void init(const std::string& fileName);
	GLuint ID() const { return textureHandler; }

	~Texture();

protected:
private:

	GLuint textureHandler;
};

