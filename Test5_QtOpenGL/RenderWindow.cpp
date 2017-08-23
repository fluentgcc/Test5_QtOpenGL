#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/Version.h>
#include <glbinding/ContextInfo.h>
#include <glbinding\Meta.h>


#include "RenderWindow.h"
#include <iostream>

#include "Root.h"

void glbinding_error(bool enable)
{
	if (enable)
	{
		glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });

		glbinding::setAfterCallback([](const glbinding::FunctionCall &)
		{
			gl::GLenum error = gl::glGetError();
			if (GL_NO_ERROR != error )
				std::cout << "Error: " << error << std::endl;
		});
	}
	else
		glbinding::setCallbackMask(glbinding::CallbackMask::None);
}

RenderWindow::RenderWindow(QWindow *parent)
	: QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
{
	this->setSurfaceType(QSurface::OpenGLSurface);

}

RenderWindow::~RenderWindow()
{
}

void RenderWindow::initializeGL()
{
	glbinding::Binding::initialize();

	glbinding_error(true);

	std::cout << std::endl
		<< "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
		<< "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
		<< "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl;

	auto ver = glbinding::ContextInfo::version();

	Graphics::Root::instance()->init();

	gl::glClearColor(0.3, 0.0, 0.3, 1.0);
}

void RenderWindow::resizeGL(int w, int h)
{
	gl::glViewport(0, 0, (gl::GLsizei)w, (gl::GLsizei)h);
}

void RenderWindow::paintGL()
{
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);


}


