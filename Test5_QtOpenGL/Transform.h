#pragma once

/***
��ʾһ�����Ա任, ƽ�ƣ� ���ţ���ת
ֻ���ڱ�ʾģ�ͱ任;
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

//------------begin:---transform���;--------------------
//getter:
		glm::mat4 getGlobalTransformMatrix() const;
		glm::mat4 getLocalTransformMatrix() const;

		//��ȡ�ֲ�λ��,��ģ�ͱ任�е�λ��)
		glm::vec3 getPosition() const;

		//��ȡ�ֲ�����ϵ��
		glm::vec3 getXAxis() const;
		glm::vec3 getYAxis() const;
		glm::vec3 getZAxis() const;
		glm::vec3 getRightDir() const { return this->getXAxis(); }		// ==getXAxis()
		glm::vec3 getUpDir() const { return this->getYAxis(); }			// ==getYAxis()
		glm::vec3 getLookAtDir() const { return -this->getZAxis(); }		// == -getZAxis()

//��ȡ��ת��; --����;
		//Pitch��: ��x�����ת;
		float getPitchRad() const;
		//Heading�ǣ� ��y�����ת;
		float getHeadingRad() const;
		//Roll�ǣ� ��z�����ת��
		float getRollRad() const;
		//��Ԫ������ת��;
		glm::quat getOrientationQuat() const;
		//ŷ���ǰ棬 ��ͬ��(Pitch, Heading, Roll);
		glm::vec3 getOrientationEulerRad() const;

		//�ֲ����ţ����ھֲ�xyz�᣻
		glm::vec3 getScale() const;

		glm::vec3 getGlobalPosition() const;
		glm::quat getGlobalQrientation() const;
		glm::vec3 getGlobalScale() const;

//setter:
		//�ֲ�λ��;
		void setPosition(const glm::vec3& pos);
		//ȫ��λ��;
		void setGlobalPosition(const glm::vec3& pos);
		void setOrientation(const glm::quat& q);
		void setOrientation(const glm::vec3& eulerAngles);
		void setGlobalOrientation(const const glm::quat& q);
		void setScale(const glm::vec3& s);
		void setScale(float s);
//Modifier:
		//�ƶ��� �ֲ�����ϵ��;
		void move(const glm::vec3& offset);
		//���� --�ֲ�����ϵ���� x�� �ƶ�;
		void trunk(float amount);
		//���� --�ֲ�����ϵ���� y�� �ƶ���
		void boom(float amount);
		//ǰ�� --�ֲ�����ϵ���� z�� �ƶ���
		void dolly(float amount);

		//��ת��
		void rotate(const glm::quat q);
		void rotateRad(float radians, const glm::vec3& v);
		//��x��;
		void tiltRad(float rad);
		//��y��;
		void panRad(float rad);
		//��z�᣻
		void rollRad(float rad);
		//��һ������ת; �ֲ�����ϵ�еĵ�;
		void rotateAround(const glm::quat& q, const glm::vec3& point);
		void rotateAroundRad(float radians, const glm::vec3& axis, const glm::vec3& point);

		//��������ϵ�еĵ�;
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


