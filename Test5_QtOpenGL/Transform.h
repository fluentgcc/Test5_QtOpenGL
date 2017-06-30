#pragma once

/***
表示一个线性变换, 平移， 缩放，旋转
只用于表示模型变换;
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
		void scale(glm::vec3& xyz_factor);
		void rotate(glm::quat& quat);


		//分解矩阵成变换 T*R*S
		glm::vec3 getTransform();
		glm::quat getRotate();
		glm::vec3 getScale();


		glm::mat4& getMat() { return this->mat_; }

		friend bool operator== (Transform& lhs, Transform& rhs);
		friend Transform operator* (Transform& lhs, Transform& rhs);
		Transform& operator= (const Transform& rhs);
	private:

		glm::mat4 mat_;
	};

}


