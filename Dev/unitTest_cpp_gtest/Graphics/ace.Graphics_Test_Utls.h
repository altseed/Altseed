
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../PCH/ace.UnitTestCpp.PCH.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void SaveSceenShot_(ace::Graphics_Imp* g, const char* file, int32_t num);

std::string GetFileName(const char* src);

void SetGLEnable(bool value);

void StartGraphicsTest();

#define SAVE_SCREEN_SHOT(g,num) { SaveSceenShot_(g,__FILE__, num); }