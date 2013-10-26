
#pragma once

#include <ace.h>

#if _WIN32
#ifdef _DEBUG
#pragma comment(lib,"Debug/ace_engine.lib")
#else
#pragma comment(lib,"Release/ace_engine.lib")
#endif
#endif