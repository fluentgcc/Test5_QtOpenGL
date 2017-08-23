#include "Buffer.h"
#include <glbinding\gl\gl.h>

#include "OpenGL\OpenGLInit.h"


using namespace Graphics;

Buffer::Data::Data()
	:id(0), size(0), lastTarget(gl::GL_ARRAY_BUFFER)
{

	if (OGL_ARB_DSA_SUPPORT)
	{
		gl::glCreateBuffers(1, &this->id);
		return;
	}

	gl::glGenBuffers(1, &this->id);
}

Buffer::Data::~Data()
{
	gl::glDeleteBuffers(1, &this->id);
}

void Buffer::create()
{
	this->data_ = std::make_shared<Data>();
}

void Buffer::create(gl::GLsizeiptr byte, gl::GLenum usage)
{
	this->create();
	this->setData(byte, 0, usage);
}

void Buffer::create(gl::GLsizeiptr byte, const void * data, gl::GLenum usage)
{
	this->create();
	this->setData(byte, data, usage);
}

bool Graphics::Buffer::isAllocated() const
{
	return this->data_.get() != nullptr;
}

gl::GLsizeiptr Graphics::Buffer::size() const
{
	if (this->data_)
	{
		return data_->size;
	}
	else
	{
		return 0;
	}
}

void Buffer::bind(gl::GLenum target) const
{
	if (data_)
	{
		gl::glBindBuffer(target, data_->id);

		data_->lastTarget = target;
		data_->isBound = true;
	}
}

void Buffer::unbind(gl::GLenum target) const
{
	gl::glBindBuffer(target, 0);
	if (data_)
	{
		data_->isBound = false;
	}
}

void Buffer::bindBase(gl::GLenum target, gl::GLuint index) const
{
	if (data_)
	{
		gl::glBindBufferBase(target, index, this->data_->id);
		data_->lastTarget = target;
		data_->isBound = true;
	}
}

void Buffer::unbindBase(gl::GLenum target, gl::GLuint index) const
{
	gl::glBindBufferBase(target, index, 0);
	if (data_)
	{
		data_->isBound = false;
	}
}

void Buffer::bindRange(gl::GLenum target, gl::GLuint index, gl::GLintptr offset, gl::GLsizeiptr size) const
{
	if (data_)
	{
		gl::glBindBufferRange(target, index, this->data_->id, offset, size);
		data_->lastTarget = target;
		data_->isBound = true;
	}
}

void Buffer::unbindRange(gl::GLenum target, gl::GLuint index) const
{
	gl::glBindBufferBase(target, index, 0);
	if (data_)
	{
		data_->isBound = false;
	}
}

gl::GLuint Buffer::getId() const
{
	if (data_)
	{
		return this->data_->id;
	}
	else
	{
		return 0;
	}
}

void Buffer::setData(gl::GLsizeiptr bytes, const void * data, gl::GLenum usage)
{
	if (!this->data_) return;

	this->data_->size = bytes;


	if (OGL_ARB_DSA_SUPPORT)
	{
		gl::glNamedBufferData(this->data_->id, bytes, data, usage);
		return;
	}

	this->bind(this->data_->lastTarget);
	gl::glBufferData(this->data_->lastTarget, bytes, data, usage);
	this->unbind(this->data_->lastTarget);
}

void Buffer::updateData(gl::GLsizeiptr bytes, const void * data)
{
	this->updateData(0, bytes, data);
}

void Buffer::updateData(gl::GLintptr offset, gl::GLsizeiptr bytes, const void * data)
{
	if (!this->data_) return;

	if (OGL_ARB_DSA_SUPPORT)
	{
		gl::glNamedBufferSubData(this->data_->id, offset, bytes, data);
		return;
	}

	this->bind(this->data_->lastTarget);
	gl::glBufferSubData(this->data_->lastTarget, offset, bytes, data);
	this->unbind(this->data_->lastTarget);
}

void* Buffer::map(gl::GLenum access)
{
	if (!this->data_) return nullptr;


	if (OGL_ARB_DSA_SUPPORT)
	{
		return gl::glMapNamedBuffer(data_->id, access);
	}
//-------------------------------
	if (!this->data_->isBound)
	{
		if (data_->lastTarget == gl::GL_PIXEL_PACK_BUFFER) 
		{
			data_->lastTarget = gl::GL_PIXEL_UNPACK_BUFFER;
		}
		else if (data_->lastTarget == gl::GL_PIXEL_UNPACK_BUFFER) 
		{
			data_->lastTarget = gl::GL_PIXEL_PACK_BUFFER;
		}
		gl::glBindBuffer(data_->lastTarget, data_->id);
	}

	auto ret = gl::glMapBuffer(data_->lastTarget, access);

	if (!data_->isBound)
	{
		unbind(data_->lastTarget);
	}

	return ret;
}

void Buffer::unmap()
{
	if (!this->data_) return;

	if (OGL_ARB_DSA_SUPPORT)
	{
		gl::glUnmapNamedBuffer(data_->id);
		return;
	}

	if(!data_->isBound)
	{
		gl::glBindBuffer(data_->lastTarget, data_->id);
	}

	gl::glUnmapBuffer(data_->lastTarget);

	if (!data_->isBound) 
	{
		this->unbind(data_->lastTarget);
	}
}

void* Buffer::mapRange(gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access)
{
	if (this->data_) return nullptr;

	if (OGL_ARB_DSA_SUPPORT)
	{
		return gl::glMapNamedBufferRange(this->data_->id, offset, length, access);
	}

	///------

	bind(data_->lastTarget);
	return gl::glMapBufferRange(data_->lastTarget, offset, length, access);
}

void Buffer::copyTo(Buffer & dstBuffer) const
{
	this->copyTo(dstBuffer, 0, 0, this->size());

}
void Buffer::copyTo(Buffer & dstBuffer, int readOffset, int writeOffset, size_t size) const
{
	if (OGL_ARB_DSA_SUPPORT)
	{
		gl::glCopyNamedBufferSubData(data_->id, dstBuffer.getId(), readOffset, writeOffset, size);
		return;
	}

	this->bind(gl::GL_COPY_READ_BUFFER);
	dstBuffer.bind(gl::GL_COPY_WRITE_BUFFER);
	gl::glCopyBufferSubData(gl::GL_COPY_READ_BUFFER, gl::GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
	this->unbind(gl::GL_COPY_READ_BUFFER);
	dstBuffer.unbind(gl::GL_COPY_WRITE_BUFFER);
}
void Buffer::invalidate()
{
	gl::glInvalidateBufferData(data_->id);
}