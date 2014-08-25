
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Window_Imp.h"
#include "../Log/ace.Log_Imp.h"

#ifdef _WIN32
#include "ace.Window_Imp_Win.h"
#else
#include "ace.Window_Imp_X11.h"
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Window_Imp::Window_Imp(Log* logger)
	: m_logger(logger)
{

}

Window_Imp::~Window_Imp()
{

}

Window_Imp* Window_Imp::Create(int32_t width, int32_t height, const achar* title, Log* logger, bool isFullScreen)
{
	Window_Imp* ret = nullptr;

	if (logger != nullptr) logger->WriteHeading("ウインドウ");

#ifdef _WIN32
	ret = Window_Imp_Win::Create(width, height, title, logger, isFullScreen);
#else
	ret = Window_Imp_X11::Create( width, height, title, logger, isFullScreen);
#endif
	if (ret != nullptr)
	{
		if (logger != nullptr) logger->WriteLine("ウインドウ作成成功");
	}
	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}