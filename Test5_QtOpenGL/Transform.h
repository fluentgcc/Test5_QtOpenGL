#pragma once

/***
��ʾһ�����Ա任, ƽ�ƣ� ���ţ���ת
һ��ֻ���ڱ�ʾģ�ͱ任;
*/

#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Object.h"

namespace Graphics
{

	class Transform :public Object
	{
	public:

		Transform() {}

		~Transform() {}

		void reset();

		void translate(glm::vec3& xyz);
		void rotate(double angle, glm::vec3& axis);
		void zoom(glm::vec3& xyz_factor);

		void rotate(glm::quat& quat);

		glm::mat4& getMat() { return this->mat_; }

		friend bool operator== (Transform& lhs, Transform& rhs);
		friend Transform operator* (Transform& lhs, Transform& rhs);

	private:

		glm::mat4 mat_;
	};


}


