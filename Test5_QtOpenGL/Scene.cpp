#include <algorithm>

#include "Scene.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"


using namespace Graphics;

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::addNode(Node * node)
{
	if (!this->hasNode(node))
	{
		this->nodes_.push_back(node);

		node->setScene(this);
	}
}

void Scene::removeNode(Node * node)
{
	auto it = std::find(this->nodes_.begin(), this->nodes_.end(), node);

	if (it != this->nodes_.end())
	{
		(*it)->setScene(nullptr);

		this->nodes_.erase(it);
	}
}

void Scene::removeNodeByName(const std::string & name)
{
	for (auto it = this->nodes_.begin(); it != this->nodes_.end();)
	{
		if ((*it)->getName() == name)
		{
			it = this->nodes_.erase(it);
		}
		else
		{
			++it;
		}
	}

}

Node * Scene::getNodeByName(const std::string & name)
{
	size_t size = this->nodes_.size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->nodes_[i]->getName() == name)
		{
			return this->nodes_[i].get();
		}
	}

	return nullptr;
}

void Scene::addLight(Light * light)
{
	if (!this->hasLight(light))
	{
		this->lights_.push_back(light);

		light->setScene(this);
	}

}

void Scene::removeLight(Light * light)
{
	auto it = std::find(this->lights_.begin(), this->lights_.end(), light);

	if (it != this->lights_.end())
	{
		(*it)->setScene(nullptr);

		this->lights_.erase(it);
	}
}

void Scene::render()
{
	//Time log begin;
	this->renderStart();
	this->renderDraw();
	this->renderFinish();

}

void Graphics::Scene::setCamera(Camera * camera)
{
	this->camera_ = camera;
}


void Scene::renderStart()
{
	//todo
	//uniform; use light;

}

void Scene::renderDraw()
{
	size_t length = this->nodes_.size();

	Transform norm;
	for (size_t i = 0; i < length; i++)
	{
		this->nodes_[i]->render(this,norm);
	}

}

void Scene::renderFinish()
{
	//todo;
}

bool Scene::hasNode(Node * node)
{
	auto it = std::find(this->nodes_.begin(), this->nodes_.end(), node);

	if (it != this->nodes_.end())
	{
		return true;
	}

	return false;
}

bool Scene::hasLight(Light * light)
{
	auto it = std::find(this->lights_.begin(), this->lights_.end(), light);

	if (it != this->lights_.end())
	{
		return true;
	}

	return false;
}


