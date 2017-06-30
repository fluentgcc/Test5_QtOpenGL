#pragma once
#include "Node.h"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Transform.h"

//��Դ�������Node, ���Լ���Node����
//��ԴҪ�����ñ�����뵽Scene��, scene->addLight(light);

//todo

namespace Graphics
{
	class Light :public Node
	{
	public:
		enum LIGHT_TYPE
		{
			AMBIENT_LIGHT

		};

	public:
		Light();
		virtual ~Light();
	

	protected:
		virtual void renderStart(Transform& model_transform) { this->parentTransorm_ = model_transform; }
		virtual void renderDraw() {};
		virtual void renderFinish() {};


	protected:
		LIGHT_TYPE lightType_;
		
		Transform parentTransorm_;

	};

	class AmbientLight : public Light
	{
	public:
		AmbientLight()
		{
			this->lightType_ = AMBIENT_LIGHT;
		}

		~AmbientLight();

	protected:
	};

}


