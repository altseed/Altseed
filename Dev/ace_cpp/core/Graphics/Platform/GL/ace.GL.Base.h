#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Graphics_Imp_GL;
	class Texture2D_Imp_GL;
	class NativeShader_Imp_GL;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static std::vector<GLenum> GLCheckError_Imp(const char *file, const int line)
{
    std::vector<GLenum> codes;
    GLenum __code = glGetError();
    while (__code != GL_NO_ERROR) {
        codes.push_back(__code);
#ifdef _DEBUG
#ifdef __APPLE__
        printf("GLError filename = %s , line = %d, error = %d\n", file, line, __code );
#else
        printf("GLError filename = %s , line = %d, error = \n", file, line );
#endif
#endif
        __code = glGetError();
    }
    return codes;
} 
#define GLCheckError() GLCheckError_Imp(__FILE__, __LINE__)
