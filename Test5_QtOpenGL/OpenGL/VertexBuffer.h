#pragma once
#include "Object.h"
#include "OpenGL\Buffer.h"

#include <opencv2\core.hpp>
#include <glbinding\gl\types.h>
#include <glm\fwd.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <map>

namespace Graphics
{
	class VertexBuffer : public Object
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer& other);
		VertexBuffer& operator=(const VertexBuffer& other);
		virtual ~VertexBuffer();

		void setIndexData(const gl::GLuint* indices, int total, int usage);

		void setVertexData(const glm::vec3* verts, int total, int usage);
		void setVertexData(const glm::vec2* verts, int total, int usage);
		void setVertexData(const float* vert0x, int numCoords, int total, int usage, int stride = 0);

		void setColorData(const glm::vec4* colors, int total, int usage);
		void setColorData(const float* color0x, int total, int usage, int stride = 0);
		void setNormalData(const glm::vec3* normals, int total, int usage);
		void setNormalData(const float* normal0x, int total, int usage, int stride = 0);
		void setTexCoordData(const glm::vec2* texCoords, int total, int usage);
		void setTexCoordData(const glm::vec3* texCoords, int total, int usage);
		void setTexCoordData(const float* texCoord0x, int numCoords, int total, int usage, int stride = 0);
		
		void setAttributeData(int location, const float * attrib0x, int numCoords, int total, int usage, int stride = 0);

//#ifndef TARGET_OPENGLES
		//glVertexAttribDivisor, 多实例绘制用到;
		void setAttributeDivisor(int location, int divisor);
//#endif

		void setVertexBuffer(Buffer& buffer, int numCoords, int stride, int offset = 0);
		void setColorBuffer(Buffer& buffer, int stride, int offset = 0);
		void setNormalBuffer(Buffer& buffer, int stride, int offset = 0);
		void setTexCoordBuffer(Buffer& buffer, int stride, int offset = 0);
		void setIndexBuffer(Buffer& buffer);

		void setAttributeBuffer(int location, Buffer& buffer, int numCoords, int stride, int offset = 0);


		Buffer& getVertexBuffer() { return positionAttribute_.buffer; }
		Buffer& getColorBuffer() { return colorAttribute_.buffer; }
		Buffer& getNormalBuffer() { return normalAttribute_.buffer; }
		Buffer& getTexCoordBuffer() { return texcoordAttribute_.buffer; }
		Buffer& getIndexBuffer() { return indexAttribute_.buffer; }
		Buffer& getAttributeBuffer(int location) { return customAttributes_.at(location).buffer; }

		const Buffer& getVertexBuffer() const { return positionAttribute_.buffer; }
		const Buffer& getColorBuffer() const { return colorAttribute_.buffer; }
		const Buffer& getNormalBuffer() const { return normalAttribute_.buffer; }
		const Buffer& getTexCoordBuffer() const { return texcoordAttribute_.buffer; }
		const Buffer& getIndexBuffer() const { return indexAttribute_.buffer; }
		const Buffer& getAttributeBuffer(int location) const { return customAttributes_.at(location).buffer; }

		void updateVertexData(const glm::vec3 * verts, int total);
		void updateVertexData(const glm::vec2 * verts, int total);
		void updateVertexData(const float * ver0x, int total);

		void updateColorData(const glm::vec4* colors, int total);
		void updateColorData(const float* color0r, int total);
		void updateNormalData(const glm::vec3* normals, int total);
		void updateNormalData(const float* normal0x, int total);
		void updateTexCoordData(const glm::vec2* texCoords, int total);
		void updateTexCoordData(const glm::vec3*  texCoords, int total);
		void updateTexCoordData(const float * texCoord0x, int total);
		void updateIndexData(const gl::GLuint* indices, int total);

		void updateAttributeData(int location, const float * attr0x, int total);

		void enableColors();
		void enableNormals();
		void enableTexCoords();
		void enableIndices();

		void disableColors();
		void disableNormals();
		void disableTexCoords();
		void disableIndices();

		gl::GLuint getVaoId() const { return this->vaoID_; }
		gl::GLuint getVertId() const { return this->positionAttribute_.getId(); }
		gl::GLuint getColorId() const { return this->colorAttribute_.getId(); }
		gl::GLuint getNormalId() const { return this->normalAttribute_.getId(); }
		gl::GLuint getTexCoordId() const { return this->texcoordAttribute_.getId(); }
		gl::GLuint getIndexId() const { return this->indexAttribute_.getId(); }
		gl::GLuint getAttributeId(int AttrPos_) const;

		bool isAllocated() const { return this->positionAttribute_.isAllocated(); }
		bool usingVerts() const { return this->usingVerts_; }
		bool usingColors() const { return this->usingColors_; }
		bool usingNormals() const { return this->usingNormals_; }
		bool usingTexCoords() const { return this->usingTexCoords_; }
		bool usingIndices() const { return this->usingIndices_; }

		void draw(gl::GLenum drawMode, int first, int total) const;
		void drawElements(gl::GLenum drawMode, int amt, int offsetelements = 0) const;
		
		void drawInstanced(gl::GLenum drawMode, int first, int total, int primCount) const;
		void drawElementsInstanced(gl::GLenum drawMode, int amt, int primCount) const;

		//draw,drawElements的简易版本;
		void render(gl::GLenum drawMode);

		void bind() const;
		void unbind() const;

		void clear();

		void clearVertices();
		void clearNormals();
		void clearColors();
		void clearTexCoords();
		void clearIndices();

		void clearAttribute(int location);

		int getNumVertices() const;
		int getNumIndices() const;

		bool hasAttribute(int location) const;

	private:

		struct VertexAttribute
		{
			VertexAttribute();
			bool isAllocated() const;
			void allocate();
			void bind() const;
			void unbind() const;
			void setData(gl::GLsizeiptr bytes, const void * data, gl::GLenum usage);
			void setData(const float* attrib0x, int numCoords, int total, int usage, int stride, bool normalize = false);
			void updateData(gl::GLintptr offset, gl::GLsizeiptr bytes, const void * data);
			void setBuffer(Buffer& buffer, int numCoords, int stride, int offset);
			void enable() const;
			void disable() const;
			gl::GLuint getId() const;

			Buffer buffer;
			gl::GLsizei stride;
			gl::GLsizeiptr offset;
			int numCoords;
			gl::GLuint location;
			bool normalize;
			int divisor;

		};

		struct IndexAttribute
		{
			IndexAttribute();
			bool isAllocated() const;
			void allocate();
			void bind() const;
			void unbind() const;
			void setData(gl::GLsizeiptr bytes, const void * data, gl::GLenum usage);
			void updateData(gl::GLintptr offset, gl::GLsizeiptr bytes, const void * data);
			gl::GLuint getId() const;

			Buffer buffer;
		};

		VertexAttribute& getOrCreateAttribute(int location);



		gl::GLuint vaoID_;
		mutable bool vaoChanged_;

		IndexAttribute indexAttribute_;
		mutable bool usingIndices_;

		VertexAttribute positionAttribute_;
		VertexAttribute colorAttribute_;
		VertexAttribute texcoordAttribute_;
		VertexAttribute normalAttribute_;
		std::map<int, VertexAttribute> customAttributes_;

		mutable bool usingVerts_; 
		mutable bool usingTexCoords_;
		mutable bool usingColors_;
		mutable bool usingNormals_;

		int totalVerts_;
		int totalIndices_;

		static bool vaoChecked_;
		static bool vaoSupported_;
	};
}
