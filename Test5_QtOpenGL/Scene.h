#pragma once

#include "Object.h"
#include <vector>


namespace Graphics
{
	class Node;
	class Camera;
	class Light;

	class Scene :public Object
	{
	public:
		Scene();
		virtual ~Scene();

		void addNode( Node* node);
		void removeNode(Node* node);

		void addLight(Light* light);
		void removeLight(Light* light);

		void render();

		Camera* getCamera() { return this->primaryCamera_.get(); }
	
	protected:
		virtual void deviceRender() = 0;


	private:
		bool hasNode(Node* act);
		bool hasLight(Light* light);

	private:
		std::vector<ref<Node>> nodes_;

		std::vector<ref<Light>> lights_;

		ref<Camera> primaryCamera_;

	};
}
