#pragma once

#include "Mesh.h"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

namespace Graphics
{
	class Cube : public Mesh
	{
	public:

		Cube(glm::vec3 size, glm::vec3 center = glm::vec3(0.0));
		~Cube();



	protected:
		void init() override;

	};
}
