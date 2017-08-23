#include "Root.h"

using namespace Graphics;

#include <glbinding\gl\gl.h>

Root::Root()
{
	this->shaderManager_ = new ShaderManager();
	this->textureManager_ = new TextureManager();

	this->init();
}

Root::~Root()
{
	if (this->shaderManager_)
	{
		delete this->shaderManager_;
		this->shaderManager_ = nullptr;
	}

	if (this->textureManager_)
	{
		delete this->textureManager_;
		this->textureManager_ = nullptr;
	}

}

void Root::init()
{
}

//------------------------------------------------------
ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::createShader(const std::string & name, const std::string & vertexShaderFile, const std::string & fragShaderFile)
{
	Shader* shader = new Shader();
	shader->loadFromFile(gl::GL_VERTEX_SHADER, vertexShaderFile);
	shader->loadFromFile(gl::GL_FRAGMENT_SHADER, fragShaderFile);
	shader->createAndLinkProgram();

	this->addShader(name, shader);

}

void ShaderManager::addShader(const std::string & name, Shader * shader)
{
	if (this->shaders_.find(name) == this->shaders_.end())
	{
		this->shaders_[name] = shader;
	}
	else 
	{
		//error: shader program name already exists;
	}
}

Shader* ShaderManager::getShader(const std::string & name)
{
	auto it = this->shaders_.find(name);
	if (it != this->shaders_.end())
	{
		return ( it->second.get());
	}
}

void ShaderManager::removeShader(const std::string & name)
{
	auto it = this->shaders_.find(name);
	if (it != this->shaders_.end())
	{
		this->shaders_.erase(it);
	}
}

void ShaderManager::removeShader(Shader * shader)
{
	for (auto it = shaders_.begin(); it != shaders_.end(); ++it)
	{
		if (it->second == shader)
		{
			it = this->shaders_.erase(it);
		}
	}
}


//--------------------------------------------------------------------

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::addTexture(const std::string & name, Texture * texture)
{
	if (this->textures_.find(name) == this->textures_.end())
	{
		this->textures_[name] = texture;
	}
	else
	{
		//error: texuter name already exists;
	}

}

Texture* TextureManager::getTexture(const std::string & name)
{
	auto it = this->textures_.find(name);
	if (it != this->textures_.end())
	{
		return (it->second.get());
	}
}

void TextureManager::removeTexture(const std::string & name)
{
	auto it = this->textures_.find(name);
	if (it != this->textures_.end())
	{
		this->textures_.erase(it);
	}
}

void TextureManager::removeTexture(Texture * texture)
{
	for (auto it = textures_.begin(); it != textures_.end(); ++it)
	{
		if (it->second == texture)
		{
			it = this->textures_.erase(it);
		}
	}
}
