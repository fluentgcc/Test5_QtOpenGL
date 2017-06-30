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
		
		//��Scenez�л�ȡ��Ҫ������;
		void setScene(Scene* scene);

		virtual void render(Transform& transform);

		void setCamera(Camera* cam);


		void setTransform(Transform* trans) { this->transform_ = trans; }
		ref<Transform> getTransform() { return this->transform_; }

		inline void setVisibility(bool b) { this->visibility_ = b; }
		inline bool getVisibility() { return this->visibility_; }

	protected:
		//��ʼ��Ⱦ; 
		virtual void renderStart(Transform& model_transform) = 0;
		virtual void renderDraw() = 0;
		virtual void renderFinish() = 0;
		bool hasChild(Node*);

	protected:
	
		//���ڵ�;��ʵӦ���ò���;
		Node* parent_;

		std::vector< ref<Node> > children_;

		//��ʾģ�ͱ任;
		ref<Transform> transform_;

		//��Camera������;
		ref<Camera> camera_;

		//�ɼ���;
		bool visibility_;

	};


};


