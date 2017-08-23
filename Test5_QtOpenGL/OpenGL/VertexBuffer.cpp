#include "VertexBuffer.h"
#include <glbinding\gl\gl.h>
#include <string.h>
#include "Shader.h"

using namespace Graphics;

bool VertexBuffer::vaoSupported_ = true;
bool VertexBuffer::vaoChecked_ = false;


//这几个静态函数其实是给 vao 加个引用计数，=0 时才删除;
static std::map<gl::GLuint, int> & getVAOIds() 
{
	static std::map<gl::GLuint, int>* ids = new std::map<gl::GLuint, int>;
	return* ids;
}

static void retainVAO(gl::GLuint id)
{
	if (id == 0) return;

	if (getVAOIds().find(id) != getVAOIds().end())
	{
		getVAOIds()[id]++;
	}
	else
	{
		getVAOIds()[id] = 1;
	}

}

static void releaseVAO(gl::GLuint id)
{
	if (getVAOIds().find(id) != getVAOIds().end())
	{
		getVAOIds()[id] --;

		if (getVAOIds()[id] == 0)
		{
			gl::glDeleteVertexArrays(1, &id);
		}
		else
		{
			//log_warning << VertexBuffer, releaseVAO():
			gl::glDeleteVertexArrays(1, &id);
		}
	}
}

//------------------------------------------------------
VertexBuffer::VertexAttribute::VertexAttribute()
	:stride(0), offset(0), numCoords(0), location(0), normalize(false), divisor(0)
{
}

bool VertexBuffer::VertexAttribute::isAllocated() const
{
	return this->buffer.isAllocated();
}

void VertexBuffer::VertexAttribute::allocate()
{
	buffer.create();
}

void VertexBuffer::VertexAttribute::bind() const
{
	buffer.bind(gl::GL_ARRAY_BUFFER);
}

void VertexBuffer::VertexAttribute::unbind() const
{
	buffer.unbind(gl::GL_ARRAY_BUFFER);
}

void VertexBuffer::VertexAttribute::setData(gl::GLsizeiptr bytes, const void * data, gl::GLenum usage)
{
	buffer.setData(bytes, data, usage);
}

void VertexBuffer::VertexAttribute::setData(const float * attrib0x, int NumCoords, int Total, int Usage, int Stride, bool Normalize)
{
	if (!isAllocated())
	{
		allocate();
	}

	gl::GLsizeiptr size = (Stride == 0) ? NumCoords * sizeof(float) : Stride;

	this->stride = size;
	this->numCoords = NumCoords;
	this->offset = 0;
	this->normalize = Normalize;
	this->setData(Total * size, attrib0x, (gl::GLenum)Usage);
}

void VertexBuffer::VertexAttribute::updateData(gl::GLintptr offset, gl::GLsizeiptr bytes, const void * data)
{
	buffer.updateData(offset, bytes, data);
}

void VertexBuffer::VertexAttribute::setBuffer(Buffer & Buffer, int NumCoords, int Stride, int Offset)
{
	this->buffer = Buffer;
	this->offset = Offset;
	this->numCoords = NumCoords;
	gl::GLsizeiptr size = (stride == 0) ? NumCoords * sizeof(float) : Stride;
	this->stride = size;
}

void VertexBuffer::VertexAttribute::enable() const
{
	this->bind();
	gl::glEnableVertexAttribArray(this->location);
	gl::glVertexAttribPointer( location, this->numCoords, gl::GL_FLOAT, normalize ? gl::GL_TRUE : gl::GL_FALSE, this->stride, (void*)this->offset );
//#ifndef TARGET_OPENGLES
	gl::glVertexAttribDivisor(location, divisor);
//#endif
	this->unbind();
}

void VertexBuffer::VertexAttribute::disable() const
{
	gl::glDisableVertexAttribArray(location);
}

gl::GLuint VertexBuffer::VertexAttribute::getId() const
{
	return buffer.getId();
}

///-------------------------------------------------------------------

VertexBuffer::IndexAttribute::IndexAttribute()
{
}

bool VertexBuffer::IndexAttribute::isAllocated() const
{
	return this->buffer.isAllocated();;
}

void VertexBuffer::IndexAttribute::allocate()
{
	buffer.create();
}

void VertexBuffer::IndexAttribute::bind() const
{
	this->buffer.bind(gl::GL_ELEMENT_ARRAY_BUFFER);
}

void VertexBuffer::IndexAttribute::unbind() const
{
	this->buffer.unbind(gl::GL_ELEMENT_ARRAY_BUFFER);
}

void VertexBuffer::IndexAttribute::setData(gl::GLsizeiptr bytes, const void * data, gl::GLenum usage)
{
	this->bind();
	buffer.setData(bytes, data, usage);
	this->unbind();
}

void VertexBuffer::IndexAttribute::updateData(gl::GLintptr offset, gl::GLsizeiptr bytes, const void * data)
{
	buffer.updateData(offset, bytes, data);
}

gl::GLuint VertexBuffer::IndexAttribute::getId() const
{
	return buffer.getId();
}

//-------------------------------------------------------------
VertexBuffer::VertexBuffer()
	:usingVerts_(false),
	usingTexCoords_(false),
	usingColors_(false),
	usingNormals_(false),
	usingIndices_(false),
	totalVerts_(0),
	totalIndices_(0),
	vaoChanged_(false),
	vaoID_(0)
{
	this->positionAttribute_.location = Shader::POSITION_ATTRIBUTE;
	this->colorAttribute_.location = Shader::COLOR_ATTRIBUTE;
	this->texcoordAttribute_.location = Shader::TEXCOORD_ATTRIBUTE;
	this->normalAttribute_.location = Shader::NORMAL_ATTRIBUTE;

}

VertexBuffer::VertexBuffer(const VertexBuffer & other)
{
	usingVerts_ = other.usingVerts_;
	usingTexCoords_ = other.usingTexCoords_;
	usingColors_ = other.usingColors_;
	usingNormals_ = other.usingNormals_;
	usingIndices_ = other.usingIndices_;

	positionAttribute_ = other.positionAttribute_;
	colorAttribute_ = other.colorAttribute_;
	texcoordAttribute_ = other.texcoordAttribute_;
	normalAttribute_ = other.normalAttribute_;
	indexAttribute_ = other.indexAttribute_;
	customAttributes_ = other.customAttributes_;

	totalVerts_ = other.totalVerts_;
	totalIndices_ = other.totalIndices_;

	vaoChanged_ = other.vaoChanged_;
	vaoID_ = other.vaoID_;

	retainVAO(vaoID_);
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer & other)
{
	if (&other == this) return *this;

	this->clear();
	usingVerts_ = other.usingVerts_;
	usingTexCoords_ = other.usingTexCoords_;
	usingColors_ = other.usingColors_;
	usingNormals_ = other.usingNormals_;
	usingIndices_ = other.usingIndices_;

	positionAttribute_ = other.positionAttribute_;
	colorAttribute_ = other.colorAttribute_;
	texcoordAttribute_ = other.texcoordAttribute_;
	normalAttribute_ = other.normalAttribute_;
	indexAttribute_ = other.indexAttribute_;
	customAttributes_ = other.customAttributes_;

	totalVerts_ = other.totalVerts_;
	totalIndices_ = other.totalIndices_;

	vaoChanged_ = other.vaoChanged_;
	vaoID_ = other.vaoID_;

	retainVAO(vaoID_);

	return *this;
}


VertexBuffer::~VertexBuffer()
{
	this->clear();
}

VertexBuffer::VertexAttribute& VertexBuffer::getOrCreateAttribute(int location)
{
	VertexAttribute * attr = nullptr;
	
	switch (location) 
	{
	case Shader::POSITION_ATTRIBUTE:
		attr = &positionAttribute_;
		break;
	case Shader::COLOR_ATTRIBUTE:
		attr = &colorAttribute_;
		break;
	case Shader::NORMAL_ATTRIBUTE:
		attr = &normalAttribute_;
		break;
	case Shader::TEXCOORD_ATTRIBUTE:
		attr = &texcoordAttribute_;
		break;
	default:
		customAttributes_[location].location = location;
		attr = &customAttributes_[location];
		vaoChanged_ = true;
		break;
	}

	return *attr;
}

void VertexBuffer::setIndexData(const gl::GLuint * indices, int total, int usage)
{
	if (!this->indexAttribute_.isAllocated())
	{
		indexAttribute_.allocate();
		this->enableIndices();
	}

	this->totalIndices_ = total;
	this->indexAttribute_.setData(sizeof(gl::GLuint) * total, indices, (gl::GLenum)usage);
}

void VertexBuffer::setVertexData(const glm::vec3 * verts, int total, int usage)
{
	this->setVertexData(&verts[0].x, 3, total, usage, sizeof(glm::vec3));
}

void VertexBuffer::setVertexData(const glm::vec2 * verts, int total, int usage)
{
	this->setVertexData(&verts[0].x, 2, total, usage, sizeof(glm::vec2));
}

void VertexBuffer::setVertexData(const float * vert0x, int numCoords, int total, int usage, int stride)
{
	this->positionAttribute_.setData(vert0x, numCoords, total, usage, stride);
	this->usingVerts_ = true;
	totalVerts_ = total;
}

void VertexBuffer::setColorData(const glm::vec4 * colors, int total, int usage)
{
	this->setColorData(&colors[0].r, total, usage, sizeof(glm::vec4));
}

void VertexBuffer::setColorData(const float * color0x, int total, int usage, int stride)
{
	this->colorAttribute_.setData(color0x, 4, total, usage, stride);
	this->enableColors();
}

void VertexBuffer::setNormalData(const glm::vec3 * normals, int total, int usage)
{
	this->setNormalData(&normals[0].x, total, usage, sizeof(glm::vec3));
}

void VertexBuffer::setNormalData(const float * normal0x, int total, int usage, int stride)
{
	this->normalAttribute_.setData(normal0x, 3, total, usage, stride);
	this->enableNormals();
}

void VertexBuffer::setTexCoordData(const glm::vec2 * texCoords, int total, int usage)
{
	this->setTexCoordData(&texCoords[0].x, 2, total, usage, sizeof(glm::vec2));
}

void VertexBuffer::setTexCoordData(const glm::vec3 * texCoords, int total, int usage)
{
	this->setTexCoordData(&texCoords[0].x, 3, total, usage, sizeof(glm::vec3));
}

void VertexBuffer::setTexCoordData(const float * texCoord0x, int numCoords, int total, int usage, int stride)
{
	this->texcoordAttribute_.setData(texCoord0x, numCoords, total, usage, stride);
}

void VertexBuffer::setAttributeData(int location, const float * attrib0x, int numCoords, int total, int usage, int stride)
{
	if (location = Shader::POSITION_ATTRIBUTE)
	{
		this->totalVerts_ = total;
	}

	bool normalize = false;

	if (!hasAttribute(location))
	{
		vaoChanged_ = true;
		usingVerts_ |= (location == Shader::POSITION_ATTRIBUTE);
		usingColors_ |= (location == Shader::COLOR_ATTRIBUTE);
		usingNormals_ |= (location == Shader::NORMAL_ATTRIBUTE);
		usingTexCoords_ |= (location == Shader::TEXCOORD_ATTRIBUTE);
	}

	this->getOrCreateAttribute(location).setData(attrib0x, numCoords, total, usage, stride, normalize);

}

void VertexBuffer::setAttributeDivisor(int location, int divisor)
{
	this->getOrCreateAttribute(location).divisor = divisor;
}

void VertexBuffer::setVertexBuffer(Buffer & buffer, int numCoords, int stride, int offset)
{
	positionAttribute_.setBuffer(buffer, numCoords, stride, offset);
	usingVerts_ = true;
	vaoChanged_ = true;
	// Calculate the total number of vertices based on what we know:
	int tmpStride = stride;
	if (tmpStride == 0) {
		// if stride is not given through argument, we need to calculate it based on 
		// on the data size and the number of coordinates.
		tmpStride = (numCoords * sizeof(float));
		if (tmpStride == 0) {
			//ofLogWarning() << "Setting buffer with 0 vertices.";
			totalVerts_ = 0;
			return;
		}
	}
	totalVerts_ = buffer.size() / tmpStride;
}

void VertexBuffer::setColorBuffer(Buffer & buffer, int stride, int offset)
{
	this->colorAttribute_.setBuffer(buffer, 4, stride, offset);
	this->enableColors();
}

void VertexBuffer::setNormalBuffer(Buffer & buffer, int stride, int offset)
{
	this->normalAttribute_.setBuffer(buffer, 3, stride,offset);
	this->enableColors();
}

void VertexBuffer::setTexCoordBuffer(Buffer & buffer, int stride, int offset)
{
	this->texcoordAttribute_.setBuffer(buffer, 4, stride, offset);
	this->enableColors();

}

void VertexBuffer::setIndexBuffer(Buffer & buffer)
{
	this->indexAttribute_.buffer = buffer;
	this->vaoChanged_ = true;
	enableIndices();
}

void VertexBuffer::setAttributeBuffer(int location, Buffer & buffer, int numCoords, int stride, int offset)
{
	if (!hasAttribute(location)) {
		vaoChanged_ = true;
		usingVerts_ |= (location == Shader::POSITION_ATTRIBUTE);
		usingColors_ |= (location == Shader::COLOR_ATTRIBUTE);
		usingNormals_ |= (location == Shader::NORMAL_ATTRIBUTE);
		usingTexCoords_ |= (location == Shader::TEXCOORD_ATTRIBUTE);
	}

	this->getOrCreateAttribute(location).setBuffer(buffer, numCoords, stride, offset);
}

void VertexBuffer::updateVertexData(const glm::vec3 * verts, int total)
{
	this->updateVertexData(&verts[0].x, total);
}

void VertexBuffer::updateVertexData(const glm::vec2 * verts, int total)
{
	this->updateVertexData(&verts[0].x, total);
}

void VertexBuffer::updateVertexData(const float * vert0x, int total)
{
	this->positionAttribute_.updateData(0, total * positionAttribute_.stride, vert0x);
}

void VertexBuffer::updateColorData(const glm::vec4 * colors, int total)
{
	this->updateColorData(&colors[0].r, total);
}

void VertexBuffer::updateColorData(const float * color0r, int total)
{
	this->colorAttribute_.updateData(0, total * colorAttribute_.stride, color0r);
}

void VertexBuffer::updateNormalData(const glm::vec3 * normals, int total)
{
	updateNormalData(&normals[0].x, total);
}

void VertexBuffer::updateNormalData(const float * normal0x, int total)
{
	this->normalAttribute_.updateData(0, total * normalAttribute_.stride, normal0x);
}

void VertexBuffer::updateTexCoordData(const glm::vec2 * texCoords, int total)
{
	updateTexCoordData(&texCoords[0].x, total);
}

void VertexBuffer::updateTexCoordData(const glm::vec3 * texCoords, int total)
{
	updateTexCoordData(&texCoords[0].x, total);
}

void VertexBuffer::updateTexCoordData(const float * texCoord0x, int total)
{
	this->texcoordAttribute_.updateData(0, total * texcoordAttribute_.stride, texCoord0x);
}

void VertexBuffer::updateIndexData(const gl::GLuint * indices, int total)
{
	if (indexAttribute_.isAllocated()) {
		indexAttribute_.updateData(0, total * sizeof(gl::GLuint), indices);
	}
}

void VertexBuffer::updateAttributeData(int location, const float * attr0x, int total)
{
	VertexAttribute * attr = nullptr;

	switch (location) {
	case Shader::POSITION_ATTRIBUTE:
		attr = &positionAttribute_;
		break;
	case Shader::COLOR_ATTRIBUTE:
		attr = &colorAttribute_;
		break;
	case Shader::NORMAL_ATTRIBUTE:
		attr = &normalAttribute_;
		break;
	case Shader::TEXCOORD_ATTRIBUTE:
		attr = &texcoordAttribute_;
		break;
	default:
		if (customAttributes_.find(location) != customAttributes_.end()) 
		{
			attr = &customAttributes_[location];
		}
		break;
	}
	

	if (attr != nullptr && attr->isAllocated()) {
		attr->updateData(0, total*attr->stride, attr0x);
	}
}

void VertexBuffer::enableColors()
{
	if (!usingColors_ && colorAttribute_.isAllocated()) 
	{
		usingColors_ = true;
		vaoChanged_ = true;
	}
}

void VertexBuffer::enableNormals()
{
	if (!usingNormals_ && normalAttribute_.isAllocated()) 
	{
		usingNormals_ = true;
		vaoChanged_ = true;
	}
}

void VertexBuffer::enableTexCoords()
{
	if (!usingTexCoords_ && texcoordAttribute_.isAllocated()) 
	{
		usingTexCoords_ = true;
		vaoChanged_ = true;
	}
}

void VertexBuffer::enableIndices()
{
	if (indexAttribute_.isAllocated() && !usingIndices_) 
	{
		usingIndices_ = true;
		vaoChanged_ = true;
	}
}

void VertexBuffer::disableColors()
{
	if (usingColors_) 
	{
		usingColors_ = false;
		vaoChanged_ = true;
	}
}

void VertexBuffer::disableNormals()
{
	if (usingNormals_) 
	{
		usingNormals_ = false;
		vaoChanged_ = true;
	}
}

void VertexBuffer::disableTexCoords()
{
	if (usingTexCoords_) 
	{
		usingTexCoords_ = false;
		vaoChanged_ = true;
	}
}

void VertexBuffer::disableIndices()
{
	if (usingIndices_) 
	{
		usingIndices_ = false;
		vaoChanged_ = true;
	}
}

gl::GLuint VertexBuffer::getAttributeId(int location) const
{
	if (!hasAttribute(location)) {
		//LogWarning() << "No attribute id found for attribute pos: " << location;
		return 0;
	}
	return const_cast<VertexBuffer*>(this)->getOrCreateAttribute(location).getId();
}

void Graphics::VertexBuffer::draw(gl::GLenum drawMode, int first, int total) const
{
	if (this->usingVerts_)
	{
		this->bind();
		gl::glDrawArrays(drawMode, first, total);
		this->unbind();
	}
}

void Graphics::VertexBuffer::drawElements(gl::GLenum drawMode, int amt, int offsetelements) const
{
	if (this->usingVerts()) 
	{
		this->bind();
		gl::glDrawElements(drawMode, amt, gl::GL_UNSIGNED_INT, (void*)(sizeof(gl::GLuint) * offsetelements));
		this->unbind();
	}
}

void Graphics::VertexBuffer::drawInstanced(gl::GLenum drawMode, int first, int total, int primCount) const
{
	if (this->usingVerts()) 
	{
		this->bind();
		gl::glDrawArraysInstanced(drawMode, first, total, primCount);
		this->unbind();
	}
}

void Graphics::VertexBuffer::drawElementsInstanced(gl::GLenum drawMode, int amt, int primCount) const
{
	if (this->usingVerts()) {
		this->bind();
		gl::glDrawElementsInstanced(drawMode, amt, gl::GL_UNSIGNED_INT, nullptr, primCount);
		this->unbind();
	}
}

void VertexBuffer::render(gl::GLenum drawMode)
{
	if (usingIndices_)
	{
		this->draw(drawMode, 0, this->totalVerts_);
	}
	else
	{
		this->drawElements(drawMode, this->totalIndices_);
	}
}

void VertexBuffer::bind() const
{
	if ((vaoSupported_ || !vaoChecked_))
	{
		if (vaoID_ == 0)
		{
			vaoChecked_ = true;
			vaoSupported_ = true;
			if (vaoSupported_)
			{
				gl::glGenVertexArrays(1, &const_cast<VertexBuffer*>(this)->vaoID_);
			}

			if (vaoID_ != 0) {
				retainVAO(vaoID_);
				vaoChanged_ = true;
			}
		}
		if (vaoSupported_)
		{
			gl::glBindVertexArray(vaoID_);
		}
	}
	else {
		vaoSupported_ = false;
	}

	if (vaoChanged_ || !vaoSupported_)
	{
		if (usingVerts_)
		{
			this->positionAttribute_.enable();
		}
		else
		{
			this->positionAttribute_.disable();
		}

		if (usingColors_)
		{
			colorAttribute_.enable();
		}
		else
		{
			colorAttribute_.disable();
		}

		if (usingNormals_)
		{
			normalAttribute_.enable();
		}
		else
		{
			normalAttribute_.disable();
		}

		if (usingTexCoords_)
		{
			texcoordAttribute_.enable();
		}
		else
		{
			texcoordAttribute_.disable();
		}

		if (usingIndices_)
		{
			indexAttribute_.bind();
		}

		std::map<int, VertexAttribute>::const_iterator it;
		for (it = customAttributes_.begin(); it != customAttributes_.end(); it++)
		{
			it->second.enable();
		}

		vaoChanged_ = false;
	}
}

void VertexBuffer::unbind() const
{
	if (vaoSupported_) 
	{
		gl::glBindVertexArray(0);
	}
	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);
	gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexBuffer::clear()
{
	this->clearVertices();
	this->clearColors();
	this->clearNormals();
	this->clearTexCoords();

	this->usingVerts_ = false;
	this->usingColors_ = false;
	this->usingNormals_ = false;
	this->usingTexCoords_ = false;

	this->customAttributes_.clear();

	this->clearIndices();

	if (vaoID_ != 0)
	{
		releaseVAO(vaoID_);
		vaoID_ = 0;
	}

}

void VertexBuffer::clearVertices()
{
	positionAttribute_ = VertexAttribute();
	positionAttribute_.location = Shader::POSITION_ATTRIBUTE;
	usingVerts_ = false;
	totalVerts_ = 0;
}

void VertexBuffer::clearNormals()
{
	normalAttribute_ = VertexAttribute();
	normalAttribute_.location = Shader::NORMAL_ATTRIBUTE;
	usingNormals_ = false;
}

void VertexBuffer::clearColors()
{
	colorAttribute_ = VertexAttribute();
	colorAttribute_.location = Shader::COLOR_ATTRIBUTE;
	usingColors_ = false;
}

void VertexBuffer::clearTexCoords()
{
	texcoordAttribute_ = VertexAttribute();
	texcoordAttribute_.location = Shader::TEXCOORD_ATTRIBUTE;
	usingTexCoords_ = false;
}

void VertexBuffer::clearIndices()
{
	if (indexAttribute_.isAllocated()) {
		indexAttribute_ = IndexAttribute();
		usingIndices_ = false;
		totalIndices_ = 0;
	}
}

void VertexBuffer::clearAttribute(int attributePos_)
{
	if (!hasAttribute(attributePos_)) return;
	if (attributePos_>3)
	{
		customAttributes_.erase(attributePos_);
	}
	else
	{
		switch (attributePos_)
		{
		case Shader::POSITION_ATTRIBUTE:
			clearVertices();
			break;
		case Shader::COLOR_ATTRIBUTE:
			clearColors();
			break;
		case Shader::NORMAL_ATTRIBUTE:
			clearNormals();
			break;
		case Shader::TEXCOORD_ATTRIBUTE:
			clearTexCoords();
			break;
		default:
			break;
		}
	}
}

int VertexBuffer::getNumVertices() const
{
	return totalVerts_;
}

int VertexBuffer::getNumIndices() const
{
	if (usingIndices_) 
	{
		return totalIndices_;
	}
	else 
	{
		return 0;
	}
}

bool VertexBuffer::hasAttribute(int location) const
{
	
	switch (location)
	{
	case Shader::POSITION_ATTRIBUTE:
		return positionAttribute_.isAllocated();
	case Shader::COLOR_ATTRIBUTE:
		return colorAttribute_.isAllocated();
	case Shader::NORMAL_ATTRIBUTE:
		return normalAttribute_.isAllocated();
	case Shader::TEXCOORD_ATTRIBUTE:
		return texcoordAttribute_.isAllocated();
	}
	
	return (customAttributes_.find(location) != customAttributes_.end());
}
