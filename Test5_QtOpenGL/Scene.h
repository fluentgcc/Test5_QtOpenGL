#pragma once

#include "Object.h"
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>


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
		
		void removeNodeByName(const std::string& name);
		Node* getNodeByName(const std::string& name);

		void setViewport(const glm::ivec4& viewport) { this->viewport_ = viewport; }
		glm::ivec4 getViewport() { return this->viewport_; }

		void addLight(Light* light);
		void removeLight(Light* light);

		void render();

		Camera* getCamera() { return this->camera_.get(); }
		void setCamera(Camera* camera);

		//π‚’’œ‡πÿ;-----
		void enableLighting();
		void disableLighting();
		void enableSeparateSpecularLight();
		void disableSeparateSpecularLight();
		bool isLightingEnabled();
		void setSmoothLighting(bool b);
		void setGlobalAmbientColor(const glm::vec4& c);
		const glm::vec4& getGlobalAmbientColor();
		//------

	protected:
		virtual void renderStart();
		virtual void renderDraw();
		virtual void renderFinish();

	private:
		bool hasNode(Node* act);
		bool hasLight(Light* light);

	private:
		std::vector<ref<Node>> nodes_;

		std::vector<ref<Light>> lights_;

		ref<Camera> camera_;

		glm::ivec4 viewport_;

	};
}
