#pragma once

/**
纹理对象;
数据用cv::Mat
现在只实现1D、2D、3D
格式支持也不完全;
*/

#include "Object.h"

#include <opencv2\core\mat.hpp>

#include <glbinding\gl\enum.h>
#include <glbinding\gl\types.h>
#include <vector>
#include "OpenGL\Buffer.h"

namespace Graphics
{ 
	class Texture : public Object
	{

	public:
		explicit Texture(bool keepData = false);
		virtual ~Texture();

		//1.参数可变式纹理创建;//texImage
		//临时; InternalFormat需要手动设;后期再修改;
		void create(gl::GLenum internalFormat, cv::InputArray arr);
		//2.创建空纹理;
		void create(gl::GLenum internalFormat, int dim, int size[], gl::GLenum format, gl::GLenum type);

		//创建缓存纹理
		void createAsBufferTexture(const Buffer & buffer, gl::GLenum glInternalFormat);

		//更新纹理；
		void copyFrom(cv::InputArray arr);
		//取纹理数据；
		void copyTo(cv::OutputArray arr) const;

		void bind() const;
		void unBind() const;

		void keepData(bool b) { this->keepData_ = b; }
		bool getKeepData() const { return this->keepData_; }
		cv::Mat getData() const;

		gl::GLuint getId() const { return this->id_; }
		gl::GLuint getBufferId() const { return this->bufferID_; }

		gl::GLenum getTarget() const { return this->target_; }
		gl::GLenum getInternalFormat() const { return this->internalFormat_; }
		gl::GLenum getSrcFormat() const { return this->srcFormat_; }
		gl::GLenum getSrcDataType() const { return this->srcDataType_; }

		std::vector<int> getSize() const;

	private:
		gl::GLuint id_;

		bool keepData_;
		cv::Mat textureData_;

		gl::GLenum target_;
		gl::GLenum internalFormat_;
		gl::GLenum srcFormat_;
		gl::GLenum srcDataType_;

		int srcMatType_;
		int srcMatDims_;

		int width_;
		int height_;
		int depth_;

		gl::GLuint bufferID_;
	};
}
