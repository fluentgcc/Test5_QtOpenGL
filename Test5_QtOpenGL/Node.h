#pragma once

#include "Object.h"
#include <vector>

namespace Graphics
{
	class Scene;
	class Camera;
	class Node;
	class Transform;



	class Node :public Object
	{
	public:
		Node();
		virtual ~Node();

		void addChild(Node* node);
		void removeChild(Node* node);
		void setParent(Node* parent) { this->parent_ = parent; }
		
		//从Scenez中获取需要的数据;
		void setScene(Scene* scene);

		virtual void render(Transform& transform);

		void setCamera(Camera* cam);


		void setTransform(Transform* trans) { this->transform_ = trans; }
		ref<Transform> getTransform() { return this->transform_; }

		inline void setVisibility(bool b) { this->visibility_ = b; }
		inline bool getVisibility() { return this->visibility_; }

	protected:
		//开始渲染; 
		virtual void renderStart(Transform& model_transform) = 0;
		virtual void renderDraw() = 0;
		virtual void renderFinish() = 0;
		bool hasChild(Node*);

	protected:
	
		//父节点;其实应该用不到;
		Node* parent_;

		std::vector< ref<Node> > children_;

		//表示模型变换;
		ref<Transform> transform_;

		//对Camera的引用;
		ref<Camera> camera_;

		//可见性;
		bool visibility_;

	};


};


