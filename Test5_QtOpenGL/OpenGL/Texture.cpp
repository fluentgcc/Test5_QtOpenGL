#include "Texture.h"
#include <glbinding\gl\gl.h>
#include <QDebug>


using namespace Graphics;

Texture::Texture(bool keepData)
	:id_(0),keepData_(keepData),bufferID_(0)
{
	
	gl::glGenTextures(1, &this->id_);
	
	if (!this->id_)
	{
		qFatal("General Texture Failed!");
	}

}

Texture::~Texture()
{
	gl::glDeleteTextures(1, &this->id_);

	this->id_ = 0;

}

void Texture::create(gl::GLenum internalFormat, cv::InputArray arr)
{
	const int kind = arr.kind();

	const cv::Size asize = arr.size();
	this->srcMatType_ = arr.type();


	const int depth = CV_MAT_DEPTH(srcMatType_);
	const int cn = CV_MAT_CN(srcMatType_);

	//CV_Assert(depth <= CV_32F);

	CV_Assert(cn == 1 || cn == 2 || cn == 3 || cn == 4);

	this->srcMatDims_ = arr.dims();
	CV_Assert(srcMatDims_ == 1 || srcMatDims_ == 2 || srcMatDims_ == 3);

	cv::Mat mat = arr.getMat();
	CV_Assert(mat.isContinuous());


	const gl::GLenum srcFormats[] =
	{
		gl::GL_NONE, gl::GL_RED, gl::GL_RG, gl::GL_RGB, gl::GL_RGBA
	};

	const gl::GLenum gl_types[] = 
	{ 
		gl::GL_UNSIGNED_BYTE, gl::GL_BYTE, gl::GL_UNSIGNED_SHORT, gl::GL_SHORT, gl::GL_INT, gl::GL_FLOAT, gl::GL_DOUBLE 
	};

	this->internalFormat_ = internalFormat;
	this->srcFormat_ = srcFormats[cn];
	this->srcDataType_ = gl_types[depth];

	switch (this->srcMatDims_)
	{
	case 1:
		this->target_ = gl::GL_TEXTURE_1D;
		this->width_ = mat.size[0];
		gl::glBindTexture((gl::GLenum)this->target_, this->id_);
		gl::glPixelStorei(gl::GL_UNPACK_ALIGNMENT, 1);
		gl::glTexImage1D((gl::GLenum)this->target_, 0, internalFormat, width_, 0, srcFormats[cn], gl_types[depth], mat.data);
		break;

	case 2:
		this->target_ = gl::GL_TEXTURE_2D;
		this->width_ = mat.size[0];
		this->height_ = mat.size[1];
		gl::glBindTexture((gl::GLenum)this->target_, this->id_);
		gl::glPixelStorei(gl::GL_UNPACK_ALIGNMENT, 1);
		gl::glTexImage2D((gl::GLenum)this->target_, 0, internalFormat, width_, height_, 0, srcFormats[cn], gl_types[depth], mat.data);
		break;

	case 3:
		this->target_ = gl::GL_TEXTURE_3D;
		this->width_ = mat.size[0];
		this->height_ = mat.size[1];
		this->depth_ = mat.size[2];
		gl::glBindTexture((gl::GLenum)this->target_, this->id_);
		gl::glPixelStorei(gl::GL_UNPACK_ALIGNMENT, 1);
		gl::glTexImage3D((gl::GLenum)this->target_, 0, internalFormat, width_, height_, depth_, 0, srcFormats[cn], gl_types[depth], mat.data);
		break;

	default:
		break;
	}

	//gl::glGenerateMipmap((gl::GLenum)this->target_);

	if (this->keepData_)
	{
		this->textureData_ = mat;
	}
}

void Texture::create(gl::GLenum internalFormat, int dim, int size[], gl::GLenum format, gl::GLenum type)
{
	this->internalFormat_ = internalFormat;
	this->srcFormat_ = format;
	this->srcDataType_ = type;

	switch (this->srcMatDims_)
	{
	case 1:
		this->target_ = gl::GL_TEXTURE_1D;
		this->width_ = size[0];
		gl::glBindTexture((gl::GLenum)this->target_, this->id_);
		gl::glPixelStorei(gl::GL_UNPACK_ALIGNMENT, 1);
		gl::glTexImage1D((gl::GLenum)this->target_, 0, internalFormat, width_, 0, srcFormat_, srcDataType_, NULL);
		break;

	case 2:
		this->target_ = gl::GL_TEXTURE_2D;
		this->width_ = size[0];
		this->height_ = size[1];
		gl::glBindTexture((gl::GLenum)this->target_, this->id_);
		gl::glPixelStorei(gl::GL_UNPACK_ALIGNMENT, 1);
		gl::glTexImage2D((gl::GLenum)this->target_, 0, internalFormat, width_, height_, 0, srcFormat_, srcDataType_, NULL);
		break;

	case 3:
		this->target_ = gl::GL_TEXTURE_3D;
		this->width_ = size[0];
		this->height_ = size[1];
		this->depth_ = size[2];
		gl::glBindTexture((gl::GLenum)this->target_, this->id_);
		gl::glPixelStorei(gl::GL_UNPACK_ALIGNMENT, 1);
		gl::glTexImage3D((gl::GLenum)this->target_, 0, internalFormat, width_, height_, depth_, 0, srcFormat_, srcDataType_, NULL);
		break;

	default:
		break;
	}

	//gl::glGenerateMipmap((gl::GLenum)this->target_);
}

void Texture::createAsBufferTexture(const Buffer & buffer, gl::GLenum glInternalFormat)
{
	this->internalFormat_ = glInternalFormat;
	this->target_ = gl::GL_TEXTURE_BUFFER;
	this->bufferID_ = buffer.getId();
	
	gl::glBindTexture((gl::GLenum)this->target_, this->id_);
	buffer.bind(gl::GL_TEXTURE_BUFFER);
}

void Texture::bind() const
{
	Q_ASSERT(this->id_);
	gl::glBindTexture((gl::GLenum)this->target_, this->id_);

}

void Texture::unBind() const
{
	gl::glBindTexture((gl::GLenum)this->target_, 0);

}

cv::Mat Texture::getData() const
{
	if (this->keepData_)
	{
		return this->textureData_;
	}
	else
	{
		return cv::Mat();
	}
}

std::vector<int> Graphics::Texture::getSize() const
{
	std::vector<int> size;

	switch (this->target_)
	{
	case gl::GL_TEXTURE_1D:
		size.push_back(this->width_);
		break;

	case gl::GL_TEXTURE_2D:
		size.push_back(this->width_);
		size.push_back(this->height_);
		break;

	case gl::GL_TEXTURE_3D:
		size.push_back(this->width_);
		size.push_back(this->height_);
		size.push_back(this->depth_);
		break;

	default:
		break;
	}

	return size;
}

void Texture::copyFrom(cv::InputArray arr)
{
	const int kind = arr.kind();

	const cv::Size asize = arr.size();
	this->srcMatType_ = arr.type();

	const int depth = CV_MAT_DEPTH(srcMatType_);
	const int cn = CV_MAT_CN(srcMatType_);

	CV_Assert(depth <= CV_32F);
	CV_Assert(cn == 1 || cn == 3 || cn == 4);


	const gl::GLenum srcFormats[] =
	{
		gl::GL_NONE, gl::GL_RED, gl::GL_RG, gl::GL_RGB, gl::GL_RGBA
	};

	const gl::GLenum gl_types[] =
	{
		gl::GL_UNSIGNED_BYTE, gl::GL_BYTE, gl::GL_UNSIGNED_SHORT, gl::GL_SHORT, gl::GL_INT, gl::GL_FLOAT, gl::GL_DOUBLE
	};

	this->srcFormat_ = srcFormats[cn];
	this->srcDataType_ = gl_types[depth];


	switch (kind)
	{
	case cv::_InputArray::OPENGL_BUFFER:
	{
		//todo
	}
	case cv::_InputArray::CUDA_GPU_MAT:
	{
		//todo
		break;
	}

	default:
	{
		cv::Mat mat = arr.getMat();
		CV_Assert(mat.isContinuous());
		gl::glBindTexture(gl::GL_TEXTURE_2D, this->id_);
		gl::glPixelStorei(gl::GL_UNPACK_ALIGNMENT, 1);
		gl::glTexSubImage2D(gl::GL_TEXTURE_2D, 0, 0, 0, this->width_, this->height_, this->srcFormat_, this->srcDataType_, mat.data);
		//gl::glGenerateMipmap(gl::GL_TEXTURE_2D);
	}
	}
}

void Texture::copyTo(cv::OutputArray arr) const
{
	this->bind();

	const int kind = arr.kind();
	switch (kind)
	{
	case cv::_InputArray::OPENGL_BUFFER:
	{
		//todo
	}
	case cv::_InputArray::CUDA_GPU_MAT:
	{
		//todo
	}
	default:
	{
		arr.create(this->width_, this->height_, this->srcMatType_);
		cv::Mat mat = arr.getMat();
		gl::glBindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

		gl::glBindTexture((gl::GLenum)this->target_, this->id_);
		gl::glPixelStorei(gl::GL_PACK_ALIGNMENT, 1);
		gl::glGetTexImage((gl::GLenum)this->target_, 0, srcFormat_, srcDataType_, mat.data);
	}
	}
	this->unBind();
}

