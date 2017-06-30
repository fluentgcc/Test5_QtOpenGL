#pragma once
/**
可绘制节点的基本类型;
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
		//开始渲染; 
		virtual void renderStart(Transform& model_transform);
		virtual void renderDraw();
		virtual void renderFinish();

	private:
		std::vector<ref<Primitive>> primitives_;

	};
}
