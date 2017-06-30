#pragma once
#include "Object.h"
#include "Texture.h"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

/**
��ʾ����, ����Primitive;
*/
namespace Graphics
{
	class Material : public Object
	{
	public:

		//alpha ��Ⱦģʽѡ��;
		enum ALPHA_MODE
		{
			OPAQUE,		//����alphaֵ�����ȫ��͸��;
			MASK,		//��ȫ��͸����͸��, �� alpha ��ֵֹ����;
			BLEND		//
		};


	public:
		Material();
		virtual ~Material();

		void begin();

		void finish();
	private:
		
		//���߿ռ䷨����ͼ;
		ref<Texture> normalTexture_;

		//�ڵ���ͼ����
		ref<Texture> occlusionTexture_;

		//�������ͼ
		ref<Texture> emissiveTexture_;

		//�������ɫ;
		glm::vec3 emissiveFactor_;

		ALPHA_MODE alphaMode_;

		//alphaMode Ϊ MASK ʱ�� Alpha ��ֵֹ;
		float alphaCutoff_; 

		//˫����;
		bool doubleSided_;

		/**pbr( Physically-Based Rendering ) ����;*/
		//���ϻ���ɫ;Ĭ��[1, 1, 1, 1];
		glm::vec4 pbrBaseColorFactor_;

		//���ϻ���ɫ��ͼ;
		ref<Texture> pbrBaseColorTexture_;

		//������Factor;
		float pbrMetallicFacor_;

		//���ϴֲڶ�;
		float pbrRoughnessFactor_;

		//�����ֲڶ���ͼ;
		ref<Texture> pbrMetallicRoughmessTexture_;
		

	};
}
