#include "Light.h"

using namespace Graphics;

Light::Light()
{
	this->glIndex_ = -1;
	this->isEnabled_ = false;
	this->attenuationConstant_ = 0.000001;
	this->attenuationLinear_ = 0.000001;
	this->attenuationQuadratic_ = 0.000001;
	this->spotCutOff_ = 45;
	this->exponent_ = 16;
	this->width_ = 1;
	this->height_ = 1;
	this->lightType_ = LIGHT_POINT;

	this->setAmbientColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
	this->setDiffuseColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	this->setSpecularColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	this->setPointLight();

	this->setAttenuation(1.0, 0.0, 0.0);


}


Light::~Light()
{
}

void Light::setup(int glindex)
{
	//this->glIndex_ = glindex;
}

void Graphics::Light::setSpotlight(float spotCutOff, float exponent)
{
	this->lightType_ = LIGHT_SPOT;
	this->setSpotlightCutOff(spotCutOff);
	this->setSpotConcentration(exponent);
}
