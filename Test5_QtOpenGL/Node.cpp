#include "Node.h"
#include <algorithm>

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>


using namespace Graphics;


Node::Node()
	:parent_(nullptr)
{
}

Node::~Node()
{
}

Node::Node(const std::string & name)
	:name_(name), parent_(nullptr)
{
}

void Node::addChild(Node * node, bool bMaintainGlobalTrans)
{
	if (node && !this->hasChild(node))
	{
		this->children_.push_back(node);
		node->setParent(this, bMaintainGlobalTrans);
	}
}


void Node::removeChild(Node * node, bool bMaintainGlobalTrans)
{
	auto it = std::find(this->children_.begin(), this->children_.end(), node);

	if (it != this->children_.end())
	{
		(*it)->clearParent(bMaintainGlobalTrans);
		(*it) = nullptr;
		this->children_.erase(it);
	}
}

void Node::removeChildByName(const std::string & node_name, bool bMaintainGlobalTrans)
{
	auto length = this->children_.size();
	for (auto it = this->children_.begin(); it != this->children_.end(); ++it)
	{
		if ( (*it)->getName() == node_name )
		{
			(*it)->clearParent(bMaintainGlobalTrans);
			(*it) = nullptr;
			it = this->children_.erase(it);
		}
	}
}

Node* Node::getChildByName(const std::string & name)
{
	auto length = this->children_.size();
	for (auto it = this->children_.begin(); it != this->children_.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			return (*it).get();
		}
	}
}

void Node::setParent(Node * parent, bool bMaintainGlobalTransform)
{
	if (this->parent_)
	{
		// we need to make sure to clear before
		// re-assigning parenthood.
		this->clearParent(bMaintainGlobalTransform);
	}
	if (bMaintainGlobalTransform) 
	{
		auto postParentPosition = position_ - parent->getGlobalPosition();
		auto postParentOrientation = orientation_ * glm::inverse(parent->getGlobalOrientation());
		auto postParentScale = scale_ / parent->getGlobalScale();
		//parent->addListener(*this);
		setOrientation(postParentOrientation);
		setPosition(postParentPosition);
		setScale(postParentScale);
	}
	else 
	{
		//parent.addListener(*this);
	}
	this->parent_ = parent;
}

void Node::clearParent(bool bMaintainGlobalTransform)
{
	if (parent_) 
	{
		//parent->removeListener(*this);
	}
	if (bMaintainGlobalTransform && parent_) 
	{
		auto orientation = getGlobalOrientation();
		auto position = getGlobalPosition();
		auto scale = getGlobalScale();
		this->parent_ = nullptr;
		setOrientation(orientation);
		setPosition(position);
		setScale(scale);
	}
	else {
		this->parent_ = nullptr;
	}
}

void Node::setScene(Scene * scene)
{
	//todo
	return;
}

void Node::render(Scene* scene, const glm::mat4& parentTransform)
{
	if (!this->visibility_)
	{
		return;
	}

	glm::mat4 trans = parentTransform * (this->localTransformMatrix_);

	this->renderStart(scene, trans);
	this->renderDraw();
	this->renderFinish();


	//子节点的绘制;
	if ( !this->children_.empty() )
	{
		size_t children_num = this->children_.size();

		for (size_t i = 0; i < children_num; i++)
		{
			if (this->children_[i].get() != nullptr)
			{
				this->children_[i]->render(scene, trans);
			}
		}
	}

}

void Node::setCamera(Camera * cam)
{
	//this->camera_ = cam;
}

bool Node::hasChild(Node * node)
{
	auto it = std::find(this->children_.begin(), this->children_.end(), node);

	if (it != this->children_.end())
	{
		return true;
	}

	return false;
}

glm::mat4 Node::getGlobalTransformMatrix() const
{
	if (this->parent_)
	{
		return parent_->getGlobalTransformMatrix() * this->getLocalTransformMatrix();
	}
	else
	{
		return this->getLocalTransformMatrix();
	}
}

const glm::mat4& Node::getLocalTransformMatrix() const
{
	return this->localTransformMatrix_;
}

glm::vec3 Node::getOrientationEulerRad() const
{
	return glm::eulerAngles(this->orientation_);
}

glm::vec3 Node::getGlobalPosition() const
{
	return getGlobalTransformMatrix()[3].xyz();
}

glm::quat Node::getGlobalOrientation() const
{
	glm::mat4 rot = glm::scale(this->getGlobalTransformMatrix(), 1.0f / this->getGlobalScale());
	return glm::toQuat(rot);
}

glm::vec3 Node::getGlobalScale() const
{
	if (parent_)
	{
		return this->scale_ * getGlobalScale();
	}
	else
	{
		return this->getScale();
	}
}

void Node::setPosition(const glm::vec3 & pos)
{
	this->position_ = pos;
	this->createMatrix();
	this->onPositionChanged();
}

void Node::setGlobalPosition(const glm::vec3 & pos)
{
	if (parent_)
	{
		auto newPos = glm::inverse(parent_->getGlobalTransformMatrix()) * glm::vec4(pos, 1.0);
		this->setPosition(newPos.xyz() / newPos.w);
	}
	else
	{
		this->setPosition(pos);
	}
}

void Node::setOrientation(const glm::quat & q)
{
	this->orientation_ = q;
	this->createMatrix();
	this->onOrientationChanged();
}

void Node::setOrientation(const glm::vec3 & eulerAngles)
{
	glm::quat q1 = glm::angleAxis(eulerAngles.x, glm::vec3(1, 0, 0));
	glm::quat q2 = glm::angleAxis(eulerAngles.z, glm::vec3(0, 0, 1));
	glm::quat q3 = glm::angleAxis(eulerAngles.y, glm::vec3(0, 1, 0));

	this->setOrientation(q1 * q2 * q3);
}

void Node::setGlobalOrientation( const glm::quat & q)
{
	if (this->parent_)
	{
		glm::mat4 invParent = glm::inverse(this->parent_->getGlobalTransformMatrix());
		glm::quat m44 = q * glm::toQuat(invParent);
		this->setOrientation(m44);
	}
	else
	{
		this->setOrientation(q);
	}
}

void Node::setScale(const glm::vec3 & s)
{
	this->scale_ = s;
	this->createMatrix();
	this->onScaleChanged();
}

void Node::move(const glm::vec3 & offset)
{
	this->position_ += offset;
	this->createMatrix();
	this->onPositionChanged();

}

void Node::rotate(const glm::quat q)
{
	this->orientation_ = q * this->orientation_;
	this->createMatrix();
	this->onOrientationChanged();
}

void Node::rotateRad(float radians, const glm::vec3 & v)
{
	this->rotate(glm::angleAxis(radians, v));
}

void Node::rotateAround(const glm::quat & q, const glm::vec3 & point)
{
	this->setGlobalPosition(q * (getGlobalPosition() - point) + point);

	this->onOrientationChanged();
	this->onPositionChanged();
}

void Node::rotateAroundRad(float radians, const glm::vec3 & axis, const glm::vec3 & point)
{
	this->rotateAround(glm::angleAxis(radians, axis), point);
}

void Node::lookAt(const glm::vec3 & lookAtPosition)
{
	glm::vec3 relPos = this->getGlobalPosition() - lookAtPosition;
	float radius = glm::length(relPos);
	if (radius > 0)
	{
		float latitude = acos(relPos.y / radius) - glm::half_pi<float>();
		float longitude = atan2(relPos.x, relPos.z);
		glm::quat q1 = glm::angleAxis(latitude, glm::vec3(1, 0, 0));
		glm::quat q2 = glm::angleAxis(longitude, glm::vec3(0, 1, 0));
		glm::quat q3 = glm::angleAxis(0.f, glm::vec3(0, 0, 1));
		glm::quat q = q1 * q2 *q3;
		this->setGlobalOrientation(q);
	}
}

void Node::lookAt(const glm::vec3 & lookAtPosition, glm::vec3 upVector)
{
	if (parent_)
	{
		glm::mat4 invParentMat = glm::inverse(parent_->getGlobalTransformMatrix()); 
		glm::vec4 upVec4 =  invParentMat * glm::vec4(upVector, 1.0);
		upVector = upVec4.xyz() / upVec4.w;
	}

	glm::vec3 zAxis = glm::normalize(this->getGlobalPosition() - lookAtPosition);
	if (glm::length(zAxis) > 0)
	{
		glm::vec3 xAxis = glm::normalize(glm::cross(upVector, zAxis));
		glm::vec3 yAxis = glm::cross(zAxis, xAxis);
		glm::mat4 m;
		m[0] = glm::vec4(xAxis, 0.f);
		m[1] = glm::vec4(yAxis, 0.f);
		m[2] = glm::vec4(zAxis, 0.f);
		
		this->setGlobalOrientation(glm::toQuat(m));
	}

}

void Node::lookAt(const Node * lookAtNode)
{
	this->lookAt(lookAtNode->getGlobalPosition());
}

void Node::lookAt(const Node * lookAtNode, const glm::vec3 & upVector)
{
	lookAt(lookAtNode->getGlobalPosition(), upVector);
}

void Node::orbitRad(float longitude, float latitude, float radius, const glm::vec3 & centerPoint)
{
	glm::quat q1 = glm::angleAxis(longitude, glm::vec3(0, 1, 0));
	glm::quat q2 = glm::angleAxis(latitude, glm::vec3(1, 0, 0));

	glm::quat q = q1 * q2;
	
	glm::vec4 p{ 0.0f, 0.0f, 1.0f, 0.0f };		//p是方向， 不是位置，所以 w = 0;

	p = q * p;									//在单位球面上旋转p;
	p = p * radius;								//按半径缩放p;

	this->setGlobalOrientation(centerPoint + p.xyz());
	this->setOrientation(q);
	onOrientationChanged();
	onPositionChanged();
}

void Node::orbitRad(float longitude, float latitude, float radius, Node * centerNode)
{
	this->orbitRad(longitude, latitude, radius, centerNode->getGlobalPosition());
}

void Node::resetTransform()
{
	this->setPosition({ 0.f,0.f,0.f });
	this->setOrientation({ 0.f,0.f,0.f });
	this->setScale({ 1.f,1.f,1.f });
}

void Node::createMatrix()
{
	this->localTransformMatrix_ = glm::translate(glm::mat4(1.0), position_);
	this->localTransformMatrix_ = localTransformMatrix_ * glm::toMat4(orientation_);
	this->localTransformMatrix_ = glm::scale(localTransformMatrix_, scale_);
	this->updateAxis();
}

void Node::updateAxis()
{
	if (scale_.x > 0)
	{
		axis_[0] = (getLocalTransformMatrix()[0] / scale_.x).xyz();
	}
	if (scale_.y > 0)
	{
		axis_[1] = (getLocalTransformMatrix()[1] / scale_.y).xyz();
	}
	if (scale_.z > 0)
	{
		axis_[2] = (getLocalTransformMatrix()[2] / scale_.z).xyz();
	}
		
}