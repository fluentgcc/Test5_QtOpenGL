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
	//emit startEvent;

	this->deviceRender();

	//emit endEvent;

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


