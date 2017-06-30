#include "Node.h"
#include <algorithm>

#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"


using namespace Graphics;


Node::Node()
{
}

Node::~Node()
{
}

void Node::addChild(Node * node)
{
	if (node && !this->hasChild(node))
	{
		this->children_.push_back(node);
		node->setParent(this);
	}
}

void Node::removeChild(Node * node)
{
	auto it = std::find(this->children_.begin(), this->children_.end(), node);

	if (it != this->children_.end())
	{
		(*it) = nullptr;
		this->children_.erase(it);
	}
}

void Node::setScene(Scene * scene)
{
	if ( !scene )
	{
		//todo
		return;
	}

	if (!this->camera_.get())
	{
		this->camera_ = scene->getCamera();
	}

	for (size_t i = 0; i < this->children_.size(); i++)
	{
		this->children_[i]->setScene(scene);
	}
}


void Node::render( Transform& transform )
{
	if (!this->visibility_)
	{
		return;
	}

	Transform trans = transform * (*this->transform_);

	if ( !this->children_.empty )
	{
		size_t children_num = this->children_.size();

		for (size_t i = 0; i < children_num; i++)
		{
			if (this->children_[i].get() != nullptr)
			{
				this->children_[i]->render(trans);
			}
		}
	}

	this->renderStart(trans);
	this->renderDraw();
	this->renderFinish();

}

void Graphics::Node::setCamera(Camera * cam)
{
}

bool Node::hasChild(Node * node)
{
	auto it = std::find(this->children_.begin(), this->children_.end(), node);

	if (it != this->children_.end() )
	{
		return true;
	}

	return false;
}
