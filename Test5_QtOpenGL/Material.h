#pragma once
#include "Object.h"

#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

/**
表示材质, 属于Primitive;
*/

namespace Graphics
{
	class Scene;
	class Texture;
	class Shader;

	class Material : public Object
	{
	public:

	public:
		Material();
		virtual ~Material() {};

		glm::vec4 getDiffuseColor() const;
		glm::vec4 getAmbientColor() const;
		glm::vec4 getSpecularColor() const;
		glm::vec4 getEmissiveColor() const;
		float getShininess() const;

		void use() const;
		void unuse() const;

		const Shader & getShader(int textureTarget, bool geometryHasColor, Scene* renderer) const;
		void uploadMatrices(const Shader* shader, Scene* renderer) const;

		void updateMaterial(const Shader* shader, Scene* renderer) const;

		void updateLights(const Shader* shader, Scene* renderer) const;
	};
}
