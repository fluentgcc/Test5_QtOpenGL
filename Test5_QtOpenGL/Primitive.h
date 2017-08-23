#pragma once

#include "Object.h"
#include "Material.h"
#include "Texture.h"

#include <vector>

#include "VertexBuffer.h"

/**
ͼԪ����;
*/

namespace Graphics
{
	class Material;

	class Primitive : public Object
	{
	public:
		Primitive();
		virtual ~Primitive();

		void render();

		void setup();

	private:
		void renderSetup();
		void renderFinish();
	private:
		
		//����
		ref<Material> material_;
		
		ref<VertexBuffer> vbo_;

		gl::GLenum renderMode_;
	};

}