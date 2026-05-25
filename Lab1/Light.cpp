#include "Light.h"

Light::Light() : position(0.0f, 10.0f, 0.0f), color(1.0f, 1.0f, 1.0f)
{

}

void Light::setPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, x);
}

void Light::setColor(float r, float g, float b)
{
	color = glm::vec3(r, b, b);
}

glm::vec3 Light::getPosition() const
{
	return position;
}

glm::vec3 Light::getColor() const
{
	return color;
}
