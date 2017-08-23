#include <glbinding/gl/gl.h>


#define __GET_VAR(x) (*(const bool*)&x)


#define OGL_ARB_DSA_SUPPORT __GET_VAR(__OPENGL_ARB_direct_state_access)



extern bool __OPENGL_ARB_direct_state_access;



static void  checkOpenGL()
{

}

