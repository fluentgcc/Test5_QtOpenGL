#pragma once
#include "Object.h"
#include <memory>
#include <vector>
#include <opencv2\core.hpp>
#include <glbinding\gl\enum.h>
#include <glbinding\gl\types.h>


//OpenGL DSA;--不用状态机;

//todo: 添加OpenCV cv::InputArray 的 create() update() 接口；


namespace Graphics
{
	class Buffer
	{
	public:
		Buffer() {};
		virtual ~Buffer() {};

		///glGenBuffers, 只创建，不分配;
		void create();

		///glGenBuffers, glNamedBufferData 创建并分配;
		void create(gl::GLsizeiptr byte, gl::GLenum usage);
		void create(gl::GLsizeiptr byte, const void* data, gl::GLenum usage);

		template <typename T>
		void create(const std::vector<T>& data, gl::GLenum usage){
			this->create(data.size() * sizeof(T), &data[0], usage);
		}

		bool isAllocated() const;

		gl::GLsizeiptr size() const;

		///glBindBuffer;
		void bind(gl::GLenum target) const;
		void unbind(gl::GLenum target) const;

//#ifndef TARGET_OPENGLES
		/// glBindBufferBase;
		void bindBase(gl::GLenum target, gl::GLuint index) const;
		void unbindBase(gl::GLenum target, gl::GLuint index) const;

		/// glBindBufferRange;
		void bindRange(gl::GLenum target, gl::GLuint index, gl::GLintptr offset, gl::GLsizeiptr size) const;
		void unbindRange(gl::GLenum target, gl::GLuint index) const;
//#endif

		gl::GLuint getId() const;
		void setData(gl::GLsizeiptr bytes, const void * data, gl::GLenum usage);

		template<typename T>
		void setData(const std::vector<T> & data, gl::GLenum usage) {
			setData(data.size() * sizeof(T), &data[0], usage);
		}

		void updateData(gl::GLsizeiptr bytes, const void* data);
		void updateData(gl::GLintptr offset, gl::GLsizeiptr bytes, const void* data);

		template<typename T>
		void updateData(gl::GLintptr offset, const std::vector<T> & data) {
			updateData(offset, data.size() * sizeof(T), &data[0]);
		}

		template<typename T>
		void updateData(const std::vector<T> & data) {
			updateData(0, data.size() * sizeof(T), &data[0]);
		}

//#ifndef TARGET_OPENGLES
		void * map(gl::GLenum access);

		template <typename T>
		T* map(gl::GLenum access){
			return static_cast<T>(this->map(access));
		}

		void unmap();

		void * mapRange(gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access);

		template<typename T>
		T * mapRange(gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access) {
			return static_cast<T*>(mapRange(offset, length, access));
		}

		void unmapRange() {
			this->unmap();
		}

		void copyTo(Buffer& dstBuffer) const;
		void copyTo(Buffer& dstBuffer, int readOffset, int writeOffset, size_t size) const;

		void invalidate();
//#endif

	private:
		struct Data
		{
			Data();
			~Data();
			gl::GLuint id;
			gl::GLsizeiptr size;
			gl::GLenum lastTarget;
			bool isBound;
		};
		std::shared_ptr<Data> data_;
	};

}