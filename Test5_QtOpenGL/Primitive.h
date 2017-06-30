#pragma once

#include "Object.h"
#include "Material.h"
#include "Texture.h"

#include <vector>
#include "Buffer.h"


/**
图元对象;
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
		
		//材质
		ref<Material> material_;


		
	};

}