#pragma once

#include <string>

/**
������������Դ����;
�Լ�ȫ��ͼ���趨.
*/


namespace Graphics
{
	class Texture;


	class Root
	{

	public:
		static void init();
		static void getProgrom(const std::string& shader_name);
		static void createProgram();

		static void registTexture(const std::string& tex_name, const Texture* texture);
		static Texture* getRegistedTexture(const std::string& tex_name);


	private:
		Root();
		~Root();
	};

}


