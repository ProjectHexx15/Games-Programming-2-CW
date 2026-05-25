#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	Light();
	Light(const glm::vec3& pos, const glm::vec3& col);

	// Setters
	void setPosition(float x, float y, float z);
	void setColor(float r, float g, float b);

	// Getters
	glm::vec3 getPosition() const;
	glm::vec3 getColor() const;

private:
	glm::vec3 position;
	glm::vec3 color;
};
