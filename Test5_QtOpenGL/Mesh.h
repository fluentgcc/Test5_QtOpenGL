#pragma once
/**
�ɻ��ƽڵ�Ļ�������;
*/

#include "Node.h"

#include <vector>


namespace Graphics
{
	class Primitive;

	class Mesh : public Node
	{
	public:
		Mesh();
		virtual ~Mesh();

	protected:
		//��ʼ��Ⱦ; 
		virtual void renderStart(Transform& model_transform);
		virtual void renderDraw();
		virtual void renderFinish();

	private:
		std::vector<ref<Primitive>> primitives_;

	};
}
