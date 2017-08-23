#include "Shader.h"

using namespace Graphics;


#include "Shader.h"
#include <iostream>
#include <fstream>
#include <glbinding/gl/gl.h>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader()
{
	this->program_ = 0;
	this->isLoaded_ = false;
	this->isBinding_ = false;
}

Shader::~Shader()
{
	this->deleteShaderProgram;
}

void Shader::deleteShaderProgram()
{
	gl::glDeleteProgram(program_);
	this->program_ = 0;
	this->shaders_.clear();
}

void Shader::loadFromString(gl::GLenum type, const std::string& source)
{
	gl::GLuint shader = gl::glCreateShader(type);

	const char* ptmp = source.c_str();
	gl::glShaderSource(shader, 1, &ptmp, NULL);

	//check whether the shader loads fine
	gl::GLint status;
	gl::glCompileShader(shader);
	gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &status);

	//gl_false;
	if (status == 0)
	{
		gl::GLint infoLogLength;
		gl::glGetShaderiv(shader, gl::GL_INFO_LOG_LENGTH, &infoLogLength);
		gl::GLchar *infoLog = new gl::GLchar[infoLogLength];
		gl::glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		std::cerr << "Compile log: " << infoLog << std::endl;
		delete[] infoLog;
	}

	this->shaders_.push_back( shader);

}

void Shader::loadFromFile(gl::GLenum whichShader, const std::string& filename)
{
	std::ifstream fp;
	fp.open(filename.c_str(), std::ios_base::in);
	if (fp)
	{
		std::string line, buffer;
		while (std::getline(fp, line))
		{
			buffer.append(line);
			buffer.append("\r\n");
		}
		//copy to source
		loadFromString(whichShader, buffer);
	}
	else {
		std::cerr << "Error loading shader: " << filename << std::endl;
	}
}

void Shader::createAndLinkProgram()
{
	program_ = gl::glCreateProgram();

	size_t length = this->shaders_.size();
	for (size_t i = 0; i < length; i++)
	{
		gl::glAttachShader(program_, shaders_[i]);
	}


	//link and check whether the program links fine
	gl::GLint status;
	gl::glLinkProgram(program_);
	gl::glGetProgramiv(program_, gl::GL_LINK_STATUS, &status);

	//GL_FALSE
	if (status == 0)
	{
		gl::GLint infoLogLength;

		gl::glGetProgramiv(program_, gl::GL_INFO_LOG_LENGTH, &infoLogLength);
		gl::GLchar *infoLog = new gl::GLchar[infoLogLength];
		gl::glGetProgramInfoLog(program_, infoLogLength, NULL, infoLog);
		std::cerr << "Link log: " << infoLog << std::endl;
		delete[] infoLog;
	}

	for (size_t i = 0; i < length; i++)
	{
		gl::glDeleteShader(shaders_[i]);
	}

	this->shaders_.clear();

	this->isLoaded_ = true;
}

void Shader::use()
{
	gl::glUseProgram(program_);
	this->isBinding_ = true;
}

void Shader::unUse()
{
	gl::glUseProgram(0);
	this->isBinding_ = false;
}

gl::GLuint Shader::getAtrributeIndex(const std::string& attribute) const
{
	gl::GLuint ind = gl::glGetAttribLocation(program_, attribute.c_str());
	return ind;
}

gl::GLuint Shader::getUniformIndex(const std::string& uniform) const
{
	gl::GLuint ind = gl::glGetUniformLocation(program_, uniform.c_str());
	return ind;
}

void Shader::setUniformTexture(const std::string & name, const Texture * texture, const gl::GLuint textureLocation) const
{
	if (this->isLoaded_)
	{
		gl::glActiveTexture(gl::GL_TEXTURE0 + textureLocation);
		gl::glBindTexture(texture->getTarget(), texture->getId());
		if (texture->getBufferId() != 0)
		{
			gl::glTexBuffer(gl::GL_TEXTURE_BUFFER, texture->getInternalFormat(), texture->getBufferId());
		}
		this->setUniform1i(name, textureLocation);
		gl::glActiveTexture(gl::GL_TEXTURE0);

	}
}

void Shader::setUniformTexture(const std::string & name, gl::GLenum textureTarget, gl::GLint textureID, int textureLocation) const
{
	if (!isLoaded_) return;

	gl::glActiveTexture(gl::GL_TEXTURE0 + textureLocation);
	gl::glBindTexture(textureTarget, textureID);
	this->setUniform1i(name, textureLocation);
	gl::glActiveTexture(gl::GL_TEXTURE0);

}

void Shader::setUniform1i(const std::string name, int i1) const
{
	if (isLoaded_) {
		int loc = this->getUniformIndex(name);
		if (loc != -1) gl::glUniform1i(loc, i1);
	}
}

void Shader::setUniform2i(const std::string name, int i1, int i2) const
{
	if (isLoaded_) {
		int loc = this->getUniformIndex(name);
		if (loc != -1) gl::glUniform2i(loc, i1, i2);
	}
}

void Shader::setUniform3i(const std::string name, int i1, int i2, int i3) const
{
	if (isLoaded_) {
		int loc = this->getUniformIndex(name);
		if (loc != -1) gl::glUniform3i(loc, i1, i2, i3);
	}
}

void Shader::setUniform4i(const std::string name, int i1, int i2, int i3, int i4) const
{
	if (isLoaded_) {
		int loc = this->getUniformIndex(name);
		if (loc != -1) gl::glUniform4i(loc, i1, i2, i3, i4);
	}
}

void Shader::setUniform1f(const std::string name, float f1) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform1f(loc, f1);
	}
}

void Shader::setUniform2f(const std::string name, float f1, float f2) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform2f(loc, f1, f2);
	}
}

void Shader::setUniform3f(const std::string name, float f1, float f2, float f3) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform3f(loc, f1, f2, f3);
	}
}

void Shader::setUniform4f(const std::string name, float f1, float f2, float f3, float f4) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform4f(loc, f1, f2, f3, f4);
	}
}

void Shader::setUniform2f(const std::string & name, const glm::vec2 & v) const
{
	this->setUniform2f(name, v.x, v.y);
}

void Shader::setUniform3f(const std::string & name, const glm::vec3 & v) const
{
	setUniform3f(name, v.x, v.y, v.z);
}

void Shader::setUniform4f(const std::string & name, const glm::vec4 & v) const
{
	setUniform4f(name, v.x, v.y, v.z, v.w);
}

void Shader::setUniform1iv(const std::string & name, const int * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform1iv(loc, count, v);
	}
}

void Shader::setUniform2iv(const std::string & name, const int * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform2iv(loc, count, v);
	}
}

void Shader::setUniform3iv(const std::string & name, const int * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform3iv(loc, count, v);
	}
}

void Shader::setUniform4iv(const std::string & name, const int * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform4iv(loc, count, v);
	}
}

void Shader::setUniform1fv(const std::string & name, const float * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform1fv(loc, count, v);
	}
}

void Shader::setUniform2fv(const std::string & name, const float * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform2fv(loc, count, v);
	}
}

void Shader::setUniform3fv(const std::string & name, const float * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform3fv(loc, count, v);
	}
}

void Shader::setUniform4fv(const std::string & name, const float * v, int count) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniform4fv(loc, count, v);
	}
}

void Shader::setUniformMatrix3f(const std::string & name, const glm::mat3 & m) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniformMatrix3fv(loc, 1, gl::GL_FALSE, glm::value_ptr(m));
	}
}

void Shader::setUniformMatrix4f(const std::string & name, const glm::mat3 & m) const
{
	if (isLoaded_) {
		int loc = getUniformIndex(name);
		if (loc != -1) gl::glUniformMatrix4fv(loc, 1, gl::GL_FALSE, glm::value_ptr(m));
	}
}
