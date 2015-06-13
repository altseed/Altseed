
#pragma once

#include <Altseed.h>

#if _WIN32
#ifdef _DEBUG
#pragma comment(lib,"Debug/Altseed.lib")
#else
#pragma comment(lib,"Release/Altseed.lib")
#endif
#endif