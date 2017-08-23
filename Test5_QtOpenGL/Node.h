#pragma once

#include "Object.h"
#include "Transform.h"
#include <vector>
#include <string>
#include <array>

#include <glm\fwd.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\quaternion.hpp>

namespace Graphics
{
	class Scene;
	class Camera;
	class Node;
	class Transform;

	class Node :public Object
	{
	public:
		Node();
		Node(const std::string& name);
		virtual ~Node();
		//Node(const Node& node);
		//Node(Node&& node);
		//Node& operator=(const Node& node);
		//Node& operator=(const Node& node);

		void setName(const std::string& name) { this->name_ = name; }
		const std::string& getName() { return this->name_; }

		void addChild(Node* node, bool bMaintainGlobalTrans = false);
		void removeChild(Node* node, bool bMaintainGlobalTrans = false);
		void removeChildByName(const std::string& node_name, bool bMaintainGlobalTrans = false);
		Node* getChildByName(const std::string& name);

		Node* getParent() { return this->parent_; }
		
		//��Scenez�л�ȡ��Ҫ������;
		void setScene(Scene* scene);
		void setCamera(Camera* cam);

		virtual void render(Scene* scene, const glm::mat4& parentTransform );

		inline void setVisibility(bool b) { this->visibility_ = b; }
		inline bool getVisibility() { return this->visibility_; }

//------------begin:---transform���;--------------------
//getter:
		glm::mat4 getGlobalTransformMatrix() const;
		const glm::mat4& getLocalTransformMatrix() const;

		//��ȡ�ֲ�λ��,��ģ�ͱ任�е�λ��)
		glm::vec3 getPosition() const { return this->position_; }

		//��ȡ�ֲ�����ϵ��
		glm::vec3 getXAxis() const { return this->axis_[0]; }
		glm::vec3 getYAxis() const { return this->axis_[1]; }
		glm::vec3 getZAxis() const { return this->axis_[2]; }
		glm::vec3 getRightDir() const { return this->getXAxis(); }		// ==getXAxis()
		glm::vec3 getUpDir() const { return this->getYAxis(); }			// ==getYAxis()
		glm::vec3 getLookAtDir() const { return -this->getZAxis(); }		// == -getZAxis()

		//��ȡ��ת��; --����;
		//Pitch��: ��x�����ת;
		float getPitchRad() const { return this->getOrientationEulerRad().x; }
		//Heading�ǣ� ��y�����ת;
		float getHeadingRad() const { return this->getOrientationEulerRad().y; }
		//Roll�ǣ� ��z�����ת��
		float getRollRad() const { return this->getOrientationEulerRad().z; }
		//��Ԫ������ת��;
		glm::quat getOrientationQuat() const { return this->orientation_; }
		//ŷ���ǰ棬 ��ͬ��(Pitch, Heading, Roll);
		glm::vec3 getOrientationEulerRad() const;

		//�ֲ����ţ����ھֲ�xyz�᣻
		glm::vec3 getScale() const { return this->scale_; }
	
		glm::vec3 getGlobalPosition() const;
		glm::quat getGlobalOrientation() const;
		glm::vec3 getGlobalScale() const;

//setter:
		//�ֲ�λ��;
		void setPosition(const glm::vec3& pos);
		//ȫ��λ��;
		void setGlobalPosition(const glm::vec3& pos);
		void setOrientation(const glm::quat& q);
		void setOrientation(const glm::vec3& eulerAngles);
		void setGlobalOrientation(const glm::quat& q);
		void setScale(const glm::vec3& s);
		void setScale(float s) { this->setScale({ s, s, s }); }
//Modifier:
		//�ƶ��� �ֲ�����ϵ��;
		void move(const glm::vec3& offset);
		//���� --�ֲ�����ϵ���� x�� �ƶ�;
		void trunk(float amount) { this->move(this->getXAxis() * amount); }
		//���� --�ֲ�����ϵ���� y�� �ƶ���
		void boom(float amount) { this->move(this->getYAxis() * amount); }
		//ǰ�� --�ֲ�����ϵ���� z�� �ƶ���
		void dolly(float amount) { this->move(this->getZAxis() * amount); }

		//��ת��
		void rotate(const glm::quat q);
		void rotateRad(float radians, const glm::vec3& v);
		//��x��;
		void tiltRad(float rad) { this->rotateRad(rad, this->getXAxis()); }
		//��y��;
		void panRad(float rad) { this->rotateRad(rad, this->getYAxis()); }
		//��z�᣻
		void rollRad(float rad) { this->rotateRad(rad, this->getZAxis()); }
		//��һ������ת; �ֲ�����ϵ�еĵ�;
		void rotateAround(const glm::quat& q, const glm::vec3& point);
		void rotateAroundRad(float radians, const glm::vec3& axis, const glm::vec3& point);

		//��������ϵ�еĵ�;
		void lookAt(const glm::vec3& lookAtPosition);
		void lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector);
		void lookAt(const Node* lookAtNode);
		void lookAt(const Node* lookAtNode, const glm::vec3& upVector);

		//����ʽ��ת;
		void orbitRad(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0));
		void orbitRad(float longitude, float latitude, float radius, Node* centerNode);
		//���ñ任;
		void resetTransform();
//---------------end;---------------------------------------

	protected:
		void createMatrix();
		void updateAxis();

		//��ʼ��Ⱦ; 
		virtual void renderStart(Scene* scene, const glm::mat4& parentTransform) = 0;
		virtual void renderDraw() = 0;
		virtual void renderFinish() = 0;
		bool hasChild(Node*);

		void setParent(Node* parent, bool bMaintainGlobalTransform = false);
		void clearParent(bool bMaintainGlobalTransform = false);

		//��λ�ı�֪ͨ;
		virtual void onPositionChanged() {}
		virtual void onOrientationChanged() {}
		virtual void onScaleChanged() {}

	protected:

		std::string name_;

		//�����Ҫ����;
		//Scene* scene_;

		//���ڵ�;
		Node* parent_;

		//�ӽڵ�;
		std::vector< ref<Node> > children_;

		//�ɼ���;
		bool visibility_;

		glm::vec3 position_;
		glm::quat orientation_;
		glm::vec3 scale_;

		std::array<glm::vec3, 3> axis_;
		glm::mat4 localTransformMatrix_;
	};
};


