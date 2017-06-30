#pragma once
#include "Object.h"
#include "Texture.h"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

/**
表示材质, 属于Primitive;
*/
namespace Graphics
{
	class Material : public Object
	{
	public:

		//alpha 渲染模式选择;
		enum ALPHA_MODE
		{
			OPAQUE,		//忽略alpha值，输出全不透明;
			MASK,		//完全不透明或透明, 由 alpha 截止值决定;
			BLEND		//
		};


	public:
		Material();
		virtual ~Material();

		void begin();

		void finish();
	private:
		
		//切线空间法线贴图;
		ref<Texture> normalTexture_;

		//遮挡贴图纹理
		ref<Texture> occlusionTexture_;

		//发射光贴图
		ref<Texture> emissiveTexture_;

		//发射光颜色;
		glm::vec3 emissiveFactor_;

		ALPHA_MODE alphaMode_;

		//alphaMode 为 MASK 时的 Alpha 截止值;
		float alphaCutoff_; 

		//双面性;
		bool doubleSided_;

		/**pbr( Physically-Based Rendering ) 参数;*/
		//材料基本色;默认[1, 1, 1, 1];
		glm::vec4 pbrBaseColorFactor_;

		//材料基本色贴图;
		ref<Texture> pbrBaseColorTexture_;

		//金属性Factor;
		float pbrMetallicFacor_;

		//材料粗糙度;
		float pbrRoughnessFactor_;

		//金属粗糙度贴图;
		ref<Texture> pbrMetallicRoughmessTexture_;
		

	};
}
