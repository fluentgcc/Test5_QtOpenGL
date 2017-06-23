#include "Transform.h"

using namespace Graphics;

void Transform::reset()
{
	this->mat_ = glm::mat4(1.0);
}
