#pragma once
#include "Node.h"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Transform.h"

//光源是特殊的Node, 可以加入Node树中
//光源要起作用必须加入到Scene中, scene->addLight(light);

//todo

namespace Graphics
{
	class Light :public Node
	{
	public:
		enum LightType
		{
			LIGHT_POINT = 0,
			LIGHT_DIRECTIONAL,
			LIGHT_SPOT,
			LIGHT_AREA
		};

	public:
		Light();
		virtual ~Light();
	
		void setup( int glindex );
		void enable() { this->isEnabled_ = true; }
		void disable() { this->isEnabled_ = false; }
		bool isEnabled() const { return this->isEnabled_; }

		void setDirectional() { this->lightType_ = LIGHT_DIRECTIONAL; }
		bool getDirectional() const { return this->lightType_ == LIGHT_DIRECTIONAL; }

		void setSpotlight(float spotCutOff = 45.f, float exponent = 0.f);
		bool isSpotlight() const { return this->lightType_ == LIGHT_SPOT; }
		void setSpotlightCutOff(float spotCutOff) { this->spotCutOff_ = spotCutOff; }
		float getSpotlightCutOff() const { return this->spotCutOff_; }
		void setSpotConcentration(float exponent) { this->exponent_ = exponent; }
		float getSpotConcentration() const { return this->exponent_; }


		void setPointLight();
		bool isPointLight() const;
		void setAttenuation(float constant = 1.f, float linear = 0.f, float quadratic = 0.f);
		float getAttenuationConstant() const;
		float getAttenuationLinear() const;
		float getAttenuationQuadratic() const;

		void setAreaLight(float width, float height);
		bool isAreaLight() const;

		int getType() const;

		void setAmbientColor(const glm::vec4& c);
		void setDiffuseColor(const glm::vec4& c);
		void setSpecularColor(const glm::vec4& c);

		glm::vec4 getAmbientColor() const;
		glm::vec4 getDiffuseColor() const;
		glm::vec4 getSpecularColor() const;

		int getLightID() const;

	private:
		Transform parentTransorm_;

		glm::vec4 ambientColor_;
		glm::vec4 diffuseColor_;
		glm::vec4 specularColor_;

		float attenuationConstant_;
		float attenuationLinear_;
		float attenuationQuadratic_;

		LightType lightType_;

		int glIndex_;
		int isEnabled_;
		float spotCutOff_;
		float exponent_;
		glm::vec4 position_;
		glm::vec3 direction_;

		//area light;
		float width_;
		float height_;
		glm::vec3 up_;
		glm::vec3 right_;


	};
}


