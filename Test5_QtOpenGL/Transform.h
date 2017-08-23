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
#include <array>

#include "Object.h"


namespace Graphics
{
	class Node;
	class Transform :public Object
	{
	public:

		Transform();

		virtual ~Transform();

//------------begin:---transform相关;--------------------
//getter:
		glm::mat4 getGlobalTransformMatrix() const;
		glm::mat4 getLocalTransformMatrix() const;

		//获取局部位置,（模型变换中的位移)
		glm::vec3 getPosition() const;

		//获取局部坐标系；
		glm::vec3 getXAxis() const;
		glm::vec3 getYAxis() const;
		glm::vec3 getZAxis() const;
		glm::vec3 getRightDir() const { return this->getXAxis(); }		// ==getXAxis()
		glm::vec3 getUpDir() const { return this->getYAxis(); }			// ==getYAxis()
		glm::vec3 getLookAtDir() const { return -this->getZAxis(); }		// == -getZAxis()

//获取旋转角; --弧度;
		//Pitch角: 绕x轴的旋转;
		float getPitchRad() const;
		//Heading角： 绕y轴的旋转;
		float getHeadingRad() const;
		//Roll角： 绕z轴的旋转；
		float getRollRad() const;
		//四元数版旋转角;
		glm::quat getOrientationQuat() const;
		//欧拉角版， 等同于(Pitch, Heading, Roll);
		glm::vec3 getOrientationEulerRad() const;

		//局部缩放，对于局部xyz轴；
		glm::vec3 getScale() const;

		glm::vec3 getGlobalPosition() const;
		glm::quat getGlobalQrientation() const;
		glm::vec3 getGlobalScale() const;

//setter:
		//局部位置;
		void setPosition(const glm::vec3& pos);
		//全局位置;
		void setGlobalPosition(const glm::vec3& pos);
		void setOrientation(const glm::quat& q);
		void setOrientation(const glm::vec3& eulerAngles);
		void setGlobalOrientation(const const glm::quat& q);
		void setScale(const glm::vec3& s);
		void setScale(float s);
//Modifier:
		//移动： 局部坐标系中;
		void move(const glm::vec3& offset);
		//左右 --局部坐标系中沿 x轴 移动;
		void trunk(float amount);
		//上下 --局部坐标系中沿 y轴 移动；
		void boom(float amount);
		//前后 --局部坐标系中沿 z轴 移动；
		void dolly(float amount);

		//旋转：
		void rotate(const glm::quat q);
		void rotateRad(float radians, const glm::vec3& v);
		//绕x轴;
		void tiltRad(float rad);
		//绕y轴;
		void panRad(float rad);
		//绕z轴；
		void rollRad(float rad);
		//绕一个点旋转; 局部坐标系中的点;
		void rotateAround(const glm::quat& q, const glm::vec3& point);
		void rotateAroundRad(float radians, const glm::vec3& axis, const glm::vec3& point);

		//世界坐标系中的点;
		void lookAt(const glm::vec3& lookAtPosition);
		void lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector);
		void lookAt(const Node* lookAtNode);
		void lookAt(const Node* lookAtNode, const glm::vec3& upVector);

		void orbitRad(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0));
		void orbitRad(float longitude, float latitude, float radius, Node* centerNode);

	private:
		glm::mat4 localTransformMat_;

		glm::vec3 position_;
		glm::quat orientation_;
		glm::vec3 scale_;

		std::array<glm::vec3, 3> axis_;
	};

}


