#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

Texture::Texture()
{
}

void Texture::init(const std::string& fileName)
{
	int width, height, numComponents; 

	// uses stb_image to load the texture file
	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4); 

	// if loading fails send an error message
	if (imageData == NULL)
	{
		std::cerr << "texture load failed" << fileName << std::endl;
		return;
	}

	// create a texture object and binds it
	glGenTextures(1, &textureHandler); 
	glBindTexture(GL_TEXTURE_2D, textureHandler); 

	// Texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

	// Texture filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); 

	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(imageData);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandler); 
}

void Texture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31); 

	glActiveTexture(GL_TEXTURE0 + unit); 
	glBindTexture(GL_TEXTURE_2D, textureHandler); 
}
