#pragma once
/**
�ɻ��ƽڵ�Ļ�������;
*/

#include "Node.h"
#include "Material.h"
#include "OpenGL\VertexBuffer.h"
#include "OpenGL\Shader.h"


#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>


namespace Graphics
{
	class Scene;

	class Mesh : public Node
	{
	public:
		Mesh();
		virtual ~Mesh();

		void setRenderMode(gl::GLenum mode) { this->mode_ = mode; }

	protected:
		//��ʼ��Ⱦ; 
		void renderStart(Scene* scene, const glm::mat4& modelTrans) override;
		void renderDraw() override;
		void renderFinish() override;

		//mesh ����ʵ��������������г�ʼ�� VertexBuffer ����
		virtual void init();

	protected:
		ref<VertexBuffer> vbo_;
		ref<Material> material_;

		gl::GLenum mode_;

		std::vector<gl::GLuint> indices_;
	};
}
