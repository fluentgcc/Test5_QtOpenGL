#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

#include "Node.h"

using namespace Graphics;

Graphics::Transform::Transform()
{
	this->setPosition({ 0.f, 0.f, 0.f });
	this->setOrientation({ 0.f, 0.f, 0.f });
	this->setScale(1.0);

}

Graphics::Transform::~Transform()
{
}
