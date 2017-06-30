#include "Transform.h"

using namespace Graphics;

void Transform::reset()
{
	this->mat_ = glm::mat4(1.0);
}

void Graphics::Transform::translate(glm::vec3 & xyz)
{
}

void Graphics::Transform::rotate(double angle, glm::vec3 & axis)
{
}

void Graphics::Transform::scale(glm::vec3 & xyz_factor)
{
}

void Graphics::Transform::rotate(glm::quat & quat)
{
}

glm::vec3 Graphics::Transform::getTransform()
{
	return glm::vec3();
}

glm::quat Graphics::Transform::getRotate()
{
	return glm::quat();
}

glm::vec3 Graphics::Transform::getScale()
{
	return glm::vec3();
}
