#include "Mesh.h"
#include "Primitive.h"
#include "Camera.h"

#include <glm/mat4x4.hpp>


using namespace Graphics;

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::renderStart(Transform & model_transform)
{
	glm::mat4 PM = this->camera_->
}

void Mesh::renderDraw()
{
}

void Mesh::renderFinish()
{
}
