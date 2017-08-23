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
		
		//从Scenez中获取需要的数据;
		void setScene(Scene* scene);
		void setCamera(Camera* cam);

		virtual void render(Scene* scene, const glm::mat4& parentTransform );

		inline void setVisibility(bool b) { this->visibility_ = b; }
		inline bool getVisibility() { return this->visibility_; }

//------------begin:---transform相关;--------------------
//getter:
		glm::mat4 getGlobalTransformMatrix() const;
		const glm::mat4& getLocalTransformMatrix() const;

		//获取局部位置,（模型变换中的位移)
		glm::vec3 getPosition() const { return this->position_; }

		//获取局部坐标系；
		glm::vec3 getXAxis() const { return this->axis_[0]; }
		glm::vec3 getYAxis() const { return this->axis_[1]; }
		glm::vec3 getZAxis() const { return this->axis_[2]; }
		glm::vec3 getRightDir() const { return this->getXAxis(); }		// ==getXAxis()
		glm::vec3 getUpDir() const { return this->getYAxis(); }			// ==getYAxis()
		glm::vec3 getLookAtDir() const { return -this->getZAxis(); }		// == -getZAxis()

		//获取旋转角; --弧度;
		//Pitch角: 绕x轴的旋转;
		float getPitchRad() const { return this->getOrientationEulerRad().x; }
		//Heading角： 绕y轴的旋转;
		float getHeadingRad() const { return this->getOrientationEulerRad().y; }
		//Roll角： 绕z轴的旋转；
		float getRollRad() const { return this->getOrientationEulerRad().z; }
		//四元数版旋转角;
		glm::quat getOrientationQuat() const { return this->orientation_; }
		//欧拉角版， 等同于(Pitch, Heading, Roll);
		glm::vec3 getOrientationEulerRad() const;

		//局部缩放，对于局部xyz轴；
		glm::vec3 getScale() const { return this->scale_; }
	
		glm::vec3 getGlobalPosition() const;
		glm::quat getGlobalOrientation() const;
		glm::vec3 getGlobalScale() const;

//setter:
		//局部位置;
		void setPosition(const glm::vec3& pos);
		//全局位置;
		void setGlobalPosition(const glm::vec3& pos);
		void setOrientation(const glm::quat& q);
		void setOrientation(const glm::vec3& eulerAngles);
		void setGlobalOrientation(const glm::quat& q);
		void setScale(const glm::vec3& s);
		void setScale(float s) { this->setScale({ s, s, s }); }
//Modifier:
		//移动： 局部坐标系中;
		void move(const glm::vec3& offset);
		//左右 --局部坐标系中沿 x轴 移动;
		void trunk(float amount) { this->move(this->getXAxis() * amount); }
		//上下 --局部坐标系中沿 y轴 移动；
		void boom(float amount) { this->move(this->getYAxis() * amount); }
		//前后 --局部坐标系中沿 z轴 移动；
		void dolly(float amount) { this->move(this->getZAxis() * amount); }

		//旋转：
		void rotate(const glm::quat q);
		void rotateRad(float radians, const glm::vec3& v);
		//绕x轴;
		void tiltRad(float rad) { this->rotateRad(rad, this->getXAxis()); }
		//绕y轴;
		void panRad(float rad) { this->rotateRad(rad, this->getYAxis()); }
		//绕z轴；
		void rollRad(float rad) { this->rotateRad(rad, this->getZAxis()); }
		//绕一个点旋转; 局部坐标系中的点;
		void rotateAround(const glm::quat& q, const glm::vec3& point);
		void rotateAroundRad(float radians, const glm::vec3& axis, const glm::vec3& point);

		//世界坐标系中的点;
		void lookAt(const glm::vec3& lookAtPosition);
		void lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector);
		void lookAt(const Node* lookAtNode);
		void lookAt(const Node* lookAtNode, const glm::vec3& upVector);

		//环绕式旋转;
		void orbitRad(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0));
		void orbitRad(float longitude, float latitude, float radius, Node* centerNode);
		//重置变换;
		void resetTransform();
//---------------end;---------------------------------------

	protected:
		void createMatrix();
		void updateAxis();

		//开始渲染; 
		virtual void renderStart(Scene* scene, const glm::mat4& parentTransform) = 0;
		virtual void renderDraw() = 0;
		virtual void renderFinish() = 0;
		bool hasChild(Node*);

		void setParent(Node* parent, bool bMaintainGlobalTransform = false);
		void clearParent(bool bMaintainGlobalTransform = false);

		//方位改变通知;
		virtual void onPositionChanged() {}
		virtual void onOrientationChanged() {}
		virtual void onScaleChanged() {}

	protected:

		std::string name_;

		//这个需要再议;
		//Scene* scene_;

		//父节点;
		Node* parent_;

		//子节点;
		std::vector< ref<Node> > children_;

		//可见性;
		bool visibility_;

		glm::vec3 position_;
		glm::quat orientation_;
		glm::vec3 scale_;

		std::array<glm::vec3, 3> axis_;
		glm::mat4 localTransformMatrix_;
	};
};


