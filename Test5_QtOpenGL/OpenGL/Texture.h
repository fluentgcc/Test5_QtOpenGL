#pragma once

/**
�������;
������cv::Mat
����ֻʵ��1D��2D��3D
��ʽ֧��Ҳ����ȫ;
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

		//1.�����ɱ�ʽ������;//texImage
		//��ʱ; InternalFormat��Ҫ�ֶ���;�������޸�;
		void create(gl::GLenum internalFormat, cv::InputArray arr);
		//2.����������;
		void create(gl::GLenum internalFormat, int dim, int size[], gl::GLenum format, gl::GLenum type);

		//������������
		void createAsBufferTexture(const Buffer & buffer, gl::GLenum glInternalFormat);

		//��������
		void copyFrom(cv::InputArray arr);
		//ȡ�������ݣ�
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
