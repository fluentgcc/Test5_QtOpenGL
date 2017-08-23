#include "Primitive.h"
#include "Material.h"


using namespace Graphics;

Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

void Graphics::Primitive::render()
{
	this->material_->use();

	this->vbo_->render(this->renderMode_);

}

