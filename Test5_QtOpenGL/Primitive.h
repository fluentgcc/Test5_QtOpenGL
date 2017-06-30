#pragma once

#include "Object.h"
#include "Material.h"
#include "Texture.h"

#include <vector>
#include "Buffer.h"


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

		void renderStart();
		void renderDraw();
		void renderFinish();

	private:
		
		//����
		ref<Material> material_;


		
	};

}