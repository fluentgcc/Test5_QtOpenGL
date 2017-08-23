#include "Mesh.h"
#include "Camera.h"

#include <glm/mat4x4.hpp>
#include "Scene.h"
#include <glbinding\gl\gl.h>
#include <glm\gtc\type_ptr.hpp>
using namespace Graphics;

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::init()
{

}

void Mesh::renderStart(Scene* scene, const glm::mat4& modelTrans)
{
	if (!this->vbo_)
	{
		return;
	}

	glm::mat4 P = scene->getCamera()->getProjectionMatrix();
	glm::mat4 V = scene->getCamera()->getViewMatrix();
	glm::mat4 PVM = P * V * modelTrans;
	

	this->material_->use();
	//gl::glUniformMatrix4fv(this->shader_->getUniformIndex("mModel"), 1, gl::GL_FALSE, glm::value_ptr(modelTrans) );
	//gl::glUniformMatrix4fv(this->shader_->getUniformIndex("mView"), 1, gl::GL_FALSE, glm::value_ptr(V));
	//gl::glUniformMatrix4fv(this->shader_->getUniformIndex("mProjection"), 1, gl::GL_FALSE, glm::value_ptr(P));
	//gl::glUniformMatrix4fv(shader->getUniformIndex("mPVM"), 1, gl::GL_FALSE, glm::value_ptr(PVM));
	this->vbo_->bind();
}

void Mesh::renderDraw()
{
	this->vbo_->render(mode_);

}

void Mesh::renderFinish()
{
	this->vbo_->unbind();
	this->material_->unuse();
}
