#pragma once
/**
可绘制节点的基本类型;
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
		//开始渲染; 
		void renderStart(Scene* scene, const glm::mat4& modelTrans) override;
		void renderDraw() override;
		void renderFinish() override;

		//mesh 必须实现这个函数，其中初始化 VertexBuffer 对象；
		virtual void init();

	protected:
		ref<VertexBuffer> vbo_;
		ref<Material> material_;

		gl::GLenum mode_;

		std::vector<gl::GLuint> indices_;
	};
}
