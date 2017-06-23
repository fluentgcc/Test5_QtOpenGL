#pragma once

#include "VertexBuffer.h"
#include "Transform.h"

namespace Graphics
{
	class Cube : public Object
	{
	public:

		void setTransform(const Transform* trans) { this->transform_ = trans; }

	private:
		VertexBuffer* vbo_;

		ref<Transform> transform_;
	};
}
