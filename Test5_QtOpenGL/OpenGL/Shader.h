#pragma once
#include "Object.h"
#include <glbinding\gl\gl.h>
#include <map>
#include <vector>
#include <glm\fwd.hpp>
#include <glm\mat4x4.hpp>
#include <glm\mat3x3.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

#include "Texture.h"

namespace Graphics
{
	class Shader : public Object
	{
	public:
		Shader();
		~Shader();

		enum defaultAttributes {
			POSITION_ATTRIBUTE = 0,  // tig: was =1, and BOY, what a performance hog this was!!! see: http://www.chromium.org/nativeclient/how-tos/3d-tips-and-best-practices
			COLOR_ATTRIBUTE,
			NORMAL_ATTRIBUTE,
			TEXCOORD_ATTRIBUTE,
			INDEX_ATTRIBUTE  // usually not used except for compute shades
		};

		void loadFromString(gl::GLenum whichShader, const std::string& source);
		void loadFromFile(gl::GLenum whichShader, const std::string& filename);
		void createAndLinkProgram();

		void use();
		void unUse();

		//An indexer that returns the location of the attribute/uniform
		//×¢ÒâÒªÏÈuse();
		gl::GLuint getAtrributeIndex(const std::string& attribute_name) const;
		gl::GLuint getUniformIndex(const std::string& uniform_name) const;

		void deleteShaderProgram();

		void setUniformTexture(const std::string& name, const Texture* texture, const gl::GLuint textureLocation) const;
		void setUniformTexture(const std::string& name, gl::GLenum textureTarget, gl::GLint textureID, int textureLocation) const;

		void setUniform1i(const std::string name, int i1) const;
		void setUniform2i(const std::string name, int i1, int i2) const;
		void setUniform3i(const std::string name, int i1, int i2, int i3) const;
		void setUniform4i(const std::string name, int i1, int i2, int i3, int i4) const;

		void setUniform1f(const std::string name, float f1) const;
		void setUniform2f(const std::string name, float f1, float f2) const;
		void setUniform3f(const std::string name, float f1, float f2, float f3) const;
		void setUniform4f(const std::string name, float f1, float f2, float f3, float f4) const;
		void setUniform2f(const std::string & name, const glm::vec2 & v) const;
		void setUniform3f(const std::string & name, const glm::vec3 & v) const;
		void setUniform4f(const std::string & name, const glm::vec4 & v) const;

		void setUniform1iv(const std::string & name, const int* v, int count = 1) const;
		void setUniform2iv(const std::string & name, const int* v, int count = 1) const;
		void setUniform3iv(const std::string & name, const int* v, int count = 1) const;
		void setUniform4iv(const std::string & name, const int* v, int count = 1) const;

		void setUniform1fv(const std::string & name, const float* v, int count = 1) const;
		void setUniform2fv(const std::string & name, const float* v, int count = 1) const;
		void setUniform3fv(const std::string & name, const float* v, int count = 1) const;
		void setUniform4fv(const std::string & name, const float* v, int count = 1) const;

		void setUniformMatrix3f(const std::string & name, const glm::mat3 & m) const;
		void setUniformMatrix4f(const std::string & name, const glm::mat3 & m) const;

	private:
		gl::GLuint	program_;

		//gl::GLuint shaders_[3];			//0->vertexshader, 1->fragmentshader, 2->geometryshader
		std::vector<gl::GLuint> shaders_;


		bool isLoaded_;
		bool isBinding_;
	};
}

