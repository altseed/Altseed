
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Graphics_Test_Utls.h"

#if _WIN32
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <sys/stat.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void SaveSceenShot_(asd::Graphics_Imp* g, const char* file, int32_t num)
{
	const char* testDirectory = "ScreenShotTest";

	auto gtype = g->GetGraphicsDeviceType();
	auto filename = GetFileName(file);

	char ssname[260];
	if (gtype == asd::GraphicsDeviceType::OpenGL)
	{
#if _WIN32
		sprintf_s(ssname, "%s/%s_%d_gl.png", testDirectory, filename.c_str(), num);
#else
		sprintf(ssname, "%s/%s_%d_gl.png", testDirectory, filename.c_str(), num);
#endif
	}
	else
	{
#if _WIN32
		sprintf_s(ssname, "%s/%s_%d_dx.png", testDirectory, filename.c_str(), num);
#else
		sprintf(ssname, "%s/%s_%d_dx.png", testDirectory, filename.c_str(), num);
#endif
	}

	g->SaveScreenshot(asd::ToAString(ssname).c_str());
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
std::string GetFileName(const char* src)
{
	auto Src = std::string(src);
	int pos = 0;
	int last = 0;
	while (src[pos] != 0)
	{
		if (src[pos] == L'\\' || src[pos] == L'/')
		{
			last = pos;
		}

		pos++;
	}

	std::string dst(&(src[last + 1]));
	return dst;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void SetGLEnable(bool value)
{
#if _WIN32
	if (value)
	{
		glfwMakeOpenGLEnabled();
	}
	else
	{
		glfwMakeOpenGLDisabled();
	}
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void StartGraphicsTest()
{
	const char* testDirectory = "ScreenShotTest";
#if _WIN32
	if (!PathIsDirectoryA(testDirectory))
	{
		CreateDirectoryA(testDirectory, NULL);
	}
#else
	mkdir( testDirectory,
		S_IRUSR | S_IWUSR | S_IXUSR |
		S_IRGRP | S_IWGRP | S_IXGRP |
		S_IROTH | S_IXOTH | S_IXOTH);
#endif
}
