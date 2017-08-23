#pragma once

#include <string>
#include <map>
#include "OpenGL\Shader.h"
#include "Texture.h"

/**
根对象，用于资源管理;
以及全局图形设定.
*/



namespace Graphics
{
	class Texture;
	class TextureManager;
	class ShaderManager;

	class Root
	{

	public:
		
		ShaderManager* shaderManager() { return this->shaderManager_; }
		TextureManager* textureManager() { return this->textureManager_; }

		Root* instance() 
		{
			static Root root;
			return &root;
		}
		void init();

		bool OPENGL_DSA_ARB;


	private:
		Root();
		~Root();
	

		ShaderManager* shaderManager_;
		TextureManager* textureManager_;
	
	};

	//管理shader程序;
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

		void createShader(const std::string& name, const std::string & vertexShaderFile, const std::string & fragShaderFile );
		void addShader(const std::string& name, Shader* shader);
		Shader* getShader(const std::string& name);
		void removeShader(const std::string& name);
		void removeShader(Shader* shader);

	private:
		std::map<std::string, ref<Shader>> shaders_;
	};

	//管理纹理对象;
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		//void createTexture();

		void addTexture(const std::string& name, Texture* texture);
		Texture* getTexture(const std::string& name);
		void removeTexture(const std::string& name);
		void removeTexture(Texture* texture);
	private:
		std::map<std::string, ref<Texture>> textures_;
	};

}


