
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>

#include <Window/ace.Window_Imp.h>
#include <Log/ace.Log_Imp.h>

#include <Graphics/Common/ace.Graphics_Imp.h>
#include <Graphics/Common/Resource/ace.VertexBuffer_Imp.h>
#include <Graphics/Common/Resource/ace.IndexBuffer_Imp.h>
#include <Graphics/Common/Resource/ace.NativeShader_Imp.h>
#include <Graphics/Common/Resource/ace.RenderState_Imp.h>
#include <Graphics/Common/Resource/ace.RenderTexture_Imp.h>
#include <Graphics/Common/Resource/ace.DepthBuffer_Imp.h>

#include<gtest/gtest.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void SaveSceenShot_(ace::Graphics_Imp* g, const char* file, int32_t num);

std::string GetFileName(const char* src);

void SetGLEnable(bool value);

void StartGraphicsTest();

#define SAVE_SCREEN_SHOT(g,num) { SaveSceenShot_(g,__FILE__, num); }

