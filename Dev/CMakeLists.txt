# Debug Releaseに限定
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# プロジェクト名
project(ace)

# CMake バージョンチェック
if (MSVC)
cmake_minimum_required(VERSION 2.8.11)
else()
cmake_minimum_required(VERSION 2.8)
endif()

# MSVC向け設定
if (MSVC)
	option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

# コンパイル設定
if (MSVC)
	# 特に何も
else()
	set(CMAKE_CXX_FLAGS "-std=c++11" ${CMAKE_CXX_FLAGS})
endif()


# ファイルをソースリストに追加
set(common_src
	ace_cpp/common/Graphics/ace.Color.cpp
	ace_cpp/common/Math/ace.Matrix33.cpp
	ace_cpp/common/Math/ace.Matrix43.cpp
	ace_cpp/common/Math/ace.Matrix44.cpp
	ace_cpp/common/Math/ace.RectF.cpp
	ace_cpp/common/Math/ace.Vector2DF.cpp
	ace_cpp/common/Math/ace.Vector2DI.cpp
	ace_cpp/common/Math/ace.Vector3DF.cpp
	ace_cpp/common/Thread/ace.CriticalSection.cpp
	ace_cpp/common/Thread/ace.Thread.cpp
	ace_cpp/common/Utility/ace.PointerArray.cpp)

set(core_src
	ace_cpp/core/ace.ReferenceObject.cpp

	ace_cpp/core/Core/ace.Core_Imp.cpp
	ace_cpp/core/Core/ace.CreateCore.cpp

	ace_cpp/core/Graphics/Common/ace.DeviceObject.cpp
	ace_cpp/core/Graphics/Common/ace.Graphics_Imp.cpp
	ace_cpp/core/Graphics/Common/ace.GraphicsResourceContainer.cpp
	ace_cpp/core/Graphics/Common/ace.RenderingThread.cpp

	ace_cpp/core/Graphics/Common/2D/ace.LayerRenderer.cpp
	ace_cpp/core/Graphics/Common/2D/ace.PostEffectRenderer.cpp
	ace_cpp/core/Graphics/Common/2D/ace.Renderer2D_Imp.cpp

	ace_cpp/core/Graphics/Common/3D/ace.Deformer_Imp.cpp
	ace_cpp/core/Graphics/Common/3D/ace.Mesh_Imp.cpp
	ace_cpp/core/Graphics/Common/3D/ace.Model_Imp
	ace_cpp/core/Graphics/Common/3D/ace.RenderedCameraObject3D.cpp
	ace_cpp/core/Graphics/Common/3D/ace.RenderedDirectionalLightObject3D.cpp
	ace_cpp/core/Graphics/Common/3D/ace.RenderedModelObject3D.cpp
	ace_cpp/core/Graphics/Common/3D/ace.RenderedObject3D.cpp
	ace_cpp/core/Graphics/Common/3D/ace.Renderer3D.cpp

	ace_cpp/core/Graphics/Common/Animation/ace.AnimationClip_Imp.cpp
	ace_cpp/core/Graphics/Common/Animation/ace.AnimationSource_Imp.cpp
	ace_cpp/core/Graphics/Common/Animation/ace.AnimationSystem_Imp.cpp
	ace_cpp/core/Graphics/Common/Animation/ace.KeyframeAnimation_Imp.cpp

	ace_cpp/core/Graphics/Common/Resource/ace.DepthBuffer_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.IndexBuffer_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.Material_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.Material2D_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.NativeShader_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.RenderState_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.RenderTexture_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.Shader2D_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.Texture2D_Imp.cpp
	ace_cpp/core/Graphics/Common/Resource/ace.VertexBuffer_Imp.cpp

	ace_cpp/core/Input/ace.Keyboard_Imp.cpp
	ace_cpp/core/Input/ace.MiddleButton.cpp
	ace_cpp/core/Input/ace.Mouse_Imp.cpp
	ace_cpp/core/Input/ace.SideButton.cpp
	ace_cpp/core/Input/ace.Joystick_Imp.cpp
	ace_cpp/core/Input/ace.JoystickContainer_Imp.cpp

	ace_cpp/core/Log/ace.Log_Imp.cpp


	ace_cpp/core/ObjectSystem/ace.CoreLayer_Imp.cpp
	ace_cpp/core/ObjectSystem/ace.CoreScene_Imp.cpp
	ace_cpp/core/ObjectSystem/ace.ObjectSystemFactory_Imp.cpp

	ace_cpp/core/ObjectSystem/2D/ace.CoreLayer2D_Imp.cpp
	ace_cpp/core/ObjectSystem/2D/ace.CoreTextureObject2D_Imp.cpp
	ace_cpp/core/ObjectSystem/2D/ace.TransformInfo2D.cpp

	ace_cpp/core/ObjectSystem/3D/ace.CoreCameraObject3D_Imp.cpp
	ace_cpp/core/ObjectSystem/3D/ace.CoreDirectionalLightObject3D_Imp.cpp
	ace_cpp/core/ObjectSystem/3D/ace.CoreLayer3D_Imp.cpp
	ace_cpp/core/ObjectSystem/3D/ace.CoreModelObject3D_Imp.cpp
	ace_cpp/core/ObjectSystem/3D/ace.CoreObject3D_Imp.cpp

	ace_cpp/core/ObjectSystem/PostEffect/ace.CorePostEffect_Imp.cpp

	ace_cpp/core/Profiler/ace.Profile.cpp
	ace_cpp/core/Profiler/ace.Profiler_Imp.cpp
	ace_cpp/core/Profiler/ace.ProfilerViewer_Imp.cpp

	ace_cpp/core/Window/ace.Window_Imp.cpp
	)

set(dx11_src
	ace_cpp/core/Graphics/DX11/ace.Graphics_Imp_DX11.cpp
	ace_cpp/core/Graphics/DX11/Resource/ace.VertexBuffer_Imp_DX11.cpp
	ace_cpp/core/Graphics/DX11/Resource/ace.IndexBuffer_Imp_DX11.cpp
	ace_cpp/core/Graphics/DX11/Resource/ace.RenderState_Imp_DX11.cpp
	ace_cpp/core/Graphics/DX11/Resource/ace.NativeShader_Imp_DX11.cpp
	ace_cpp/core/Graphics/DX11/Resource/ace.Texture2D_Imp_DX11.cpp
	ace_cpp/core/Graphics/DX11/Resource/ace.RenderTexture_Imp_DX11.cpp
	ace_cpp/core/Graphics/DX11/Resource/ace.DepthBuffer_Imp_DX11.cpp)

set(gl_src
	ace_cpp/core/Graphics/GL/ace.Graphics_Imp_GL.cpp
	ace_cpp/core/Graphics/GL/Resource/ace.VertexBuffer_Imp_GL.cpp
	ace_cpp/core/Graphics/GL/Resource/ace.IndexBuffer_Imp_GL.cpp
	ace_cpp/core/Graphics/GL/Resource/ace.RenderState_Imp_GL.cpp
	ace_cpp/core/Graphics/GL/Resource/ace.NativeShader_Imp_GL.cpp
	ace_cpp/core/Graphics/GL/Resource/ace.Texture2D_Imp_GL.cpp
	ace_cpp/core/Graphics/GL/Resource/ace.RenderTexture_Imp_GL.cpp
	ace_cpp/core/Graphics/GL/Resource/ace.DepthBuffer_Imp_GL.cpp)

if (WIN32)
	list(APPEND core_src
			ace_cpp/core/Window/ace.Window_Imp_Win.cpp)

	list(APPEND core_src ${dx11_src})
	list(APPEND core_src ${gl_src})
else()
	list(APPEND core_src
		ace_cpp/core/Window/ace.Window_Imp_X11.cpp)
	list(APPEND core_src ${gl_src})
endif()

set(engine_src
	ace_cpp/engine/ace.Engine.cpp

	ace_cpp/engine/ObjectSystem/ace.Layer.cpp
	ace_cpp/engine/ObjectSystem/ace.Scene.cpp

	ace_cpp/engine/ObjectSystem/2D/ace.Layer2D.cpp
	ace_cpp/engine/ObjectSystem/2D/ace.Object2D.cpp
	ace_cpp/engine/ObjectSystem/2D/ace.TextureObject2D.cpp

	ace_cpp/engine/ObjectSystem/3D/ace.CameraObject3D.cpp
	ace_cpp/engine/ObjectSystem/3D/ace.DirectionalLightObject3D.cpp
	ace_cpp/engine/ObjectSystem/3D/ace.Layer3D.cpp
	ace_cpp/engine/ObjectSystem/3D/ace.ModelObject3D.cpp
	ace_cpp/engine/ObjectSystem/3D/ace.Object3D.cpp

	ace_cpp/engine/ObjectSystem/Component/ace.Layer2DComponent.cpp
	ace_cpp/engine/ObjectSystem/Component/ace.Object2DComponent.cpp
	ace_cpp/engine/ObjectSystem/Component/ace.SceneComponent.cpp

	ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffect.cpp
	ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectGaussianBlur.cpp
	ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectGrayScale.cpp
	ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectSepia.cpp
)

set(unittest_gtest_src
	unitTest_cpp_gtest/ace.common.Base_test.cpp
	unitTest_cpp_gtest/ace.DLL.cpp
	unitTest_cpp_gtest/main.cpp

	unitTest_cpp_gtest/Graphics/ace.Graphics_Test_Utls.cpp

	unitTest_cpp_gtest/Graphics/2D/ace.LayerRenderer_Test.cpp
	unitTest_cpp_gtest/Graphics/2D/ace.PostEffectRenderer_Test.cpp
	unitTest_cpp_gtest/Graphics/2D/ace.Renderer2D_Test.cpp

	unitTest_cpp_gtest/Graphics/3D/ace.Mesh_Test.cpp

	unitTest_cpp_gtest/Graphics/LowLayer/ace.Graphics_Empty_Test.cpp
	unitTest_cpp_gtest/Graphics/LowLayer/ace.Graphics_RenderTarget.cpp
	unitTest_cpp_gtest/Graphics/LowLayer/ace.Graphics_Sampler_Test.cpp
	unitTest_cpp_gtest/Graphics/LowLayer/ace.Graphics_Simple3D_Test.cpp
	unitTest_cpp_gtest/Graphics/LowLayer/ace.Graphics_SingleTexture_Test.cpp

	unitTest_cpp_gtest/Log/ace.Log_Test.cpp

	unitTest_cpp_gtest/Profiler/ace.ProfilerViewer_test.cpp

	unitTest_cpp_gtest/Utility/ace.CopyArrayTest.cpp
	unitTest_cpp_gtest/Utility/ace.BinaryRederWriterTest.cpp
	unitTest_cpp_gtest/Utility/ace.GetTypeNameTest.cpp
	unitTest_cpp_gtest/Utility/ace.PointerArray_test.cpp
	unitTest_cpp_gtest/Utility/ace.Timer_test.cpp
	unitTest_cpp_gtest/Utility/ace.TypeErasureCopy_test.cpp

	unitTest_cpp_gtest/Window/ace.Window_Empty_Test.cpp)

set(unittest_engine_gtest_src
	unitTest_Engine_cpp_gtest/EngineTest.cpp
	unitTest_Engine_cpp_gtest/main.cpp
	unitTest_Engine_cpp_gtest/Profiler.cpp

	unitTest_Engine_cpp_gtest/ObjectSystem/Component.cpp
	unitTest_Engine_cpp_gtest/ObjectSystem/CustomObject.cpp
	unitTest_Engine_cpp_gtest/ObjectSystem/LayerComponent.cpp
	unitTest_Engine_cpp_gtest/ObjectSystem/LayerHasNoObject.cpp
	unitTest_Engine_cpp_gtest/ObjectSystem/LayersMember.cpp
	unitTest_Engine_cpp_gtest/ObjectSystem/Object2DTest.cpp
	unitTest_Engine_cpp_gtest/ObjectSystem/ObjectsMember.cpp
	unitTest_Engine_cpp_gtest/ObjectSystem/ParentObject.cpp

	unitTest_Engine_cpp_gtest/Graphics/3D/SimpleMesh.cpp

	unitTest_Engine_cpp_gtest/Graphics/PostEffect/CustomPostEffect.cpp
	unitTest_Engine_cpp_gtest/Graphics/PostEffect/PostEffectGaussinaBlurTest.cpp
	unitTest_Engine_cpp_gtest/Graphics/PostEffect/PostEffectGrayScaleTest.cpp
	unitTest_Engine_cpp_gtest/Graphics/PostEffect/PostEffectSepiaTest.cpp

)

# インクルードの設定
include_directories(
	include/
	ace_cpp/common/
	ace_cpp/core/
	)

#ライブラリの設定
link_directories(${PROJECT_SOURCE_DIR}/lib/)

# パッケージの設定
set(common_lib)
set(common_inc)

if(MSVC)

else()
	list(APPEND common_lib glfw3 )
	list(APPEND common_lib GLEW )
	list(APPEND common_lib png16 )
	list(APPEND common_lib z )
	list(APPEND common_lib gtest )
	list(APPEND common_lib gtest_main )

	list(APPEND common_lib dl )

	find_package(Threads REQUIRED)
	list(APPEND common_lib ${CMAKE_THREAD_LIBS_INIT})

	find_package(X11 REQUIRED)
	list(APPEND common_inc ${X11_INCLUDE_DIR})
	list(APPEND common_lib ${X11_LIBRARIES})

	find_package(OpenGL)
	list(APPEND common_inc ${OPENGL_INCLUDE_DIR})
	list(APPEND common_lib ${OPENGL_LIBRARY})

	if (NOT X11_Xrandr_FOUND)
		message(FATAL_ERROR "X11_Xrandr were not found")
	endif()
	list(APPEND common_inc ${X11_Xrandr_INCLUDE_PATH})
	list(APPEND common_lib ${X11_Xrandr_LIB})

	if (NOT X11_Xinput_FOUND)
		message(FATAL_ERROR "X11_Xinput were not found")
	endif()
	list(APPEND common_inc ${X11_Xinput_INCLUDE_PATH})
	list(APPEND common_lib ${X11_Xinput_LIB})

	if (NOT X11_xf86vmode_FOUND)
		message(FATAL_ERROR "X11_xf86vmode were not found")
	endif()

	list(APPEND common_inc ${X11_xf86vmode_INCLUDE_PATH})
	list(APPEND common_lib ${X11_Xxf86vm_LIB})

endif()

# コンパイル先を追加
add_library( aceCommon STATIC ${common_src} )

add_library( aceCore STATIC ${core_src} )
add_library( aceCoreDLL SHARED ${core_src} )
target_link_libraries(aceCore aceCommon) 
target_link_libraries(aceCoreDLL aceCommon ${common_lib}) 

add_library( aceEngine STATIC ${engine_src} ${common_src} )

add_executable( aceUnitTestGtest ${unittest_gtest_src} )
target_link_libraries(aceUnitTestGtest aceCore ${common_lib}) 

add_executable( aceUnitTestEngineGtest ${unittest_engine_gtest_src} )
target_link_libraries(aceUnitTestEngineGtest aceEngine ${common_lib}) 



# 出力先の設定
set_target_properties(aceCommon PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(aceCommon PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(aceCore PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(aceCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(aceCoreDLL PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "bin/" )
set_target_properties(aceCoreDLL PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )
set_target_properties(aceCoreDLL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )

set_target_properties(aceEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(aceEngine PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(aceUnitTestGtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )
set_target_properties(aceUnitTestEngineGtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )

if (MSVC)
set_target_properties(aceCommon PROPERTIES OUTPUT_NAME_DEBUG "ace_common" )
set_target_properties(aceCommon PROPERTIES OUTPUT_NAME_RELEASE "ace_common" )
else()
set_target_properties(aceCommon PROPERTIES OUTPUT_NAME "ace_common" )
endif()

if (MSVC)
set_target_properties(aceCore PROPERTIES OUTPUT_NAME_DEBUG "ace_core" )
set_target_properties(aceCore PROPERTIES OUTPUT_NAME_RELEASE "ace_core" )
else()
set_target_properties(aceCore PROPERTIES OUTPUT_NAME "ace_core" )
endif()

if (MSVC)
set_target_properties(aceCoreDLL PROPERTIES OUTPUT_NAME_DEBUG "ace_core" )
set_target_properties(aceCoreDLL PROPERTIES OUTPUT_NAME_RELEASE "ace_core" )
else()
set_target_properties(aceCoreDLL PROPERTIES OUTPUT_NAME "ace_core" )
endif()

if (MSVC)
set_target_properties(aceEngine PROPERTIES OUTPUT_NAME_DEBUG "ace_engine" )
set_target_properties(aceEngine PROPERTIES OUTPUT_NAME_RELEASE "ace_engine" )
else()
set_target_properties(aceEngine PROPERTIES OUTPUT_NAME "ace_engine" )
endif()

set_target_properties(aceUnitTestGtest PROPERTIES OUTPUT_NAME_DEBUG "UnitTestGtest_cpp" )
set_target_properties(aceUnitTestGtest PROPERTIES OUTPUT_NAME_RELEASE "UnitTestGtest_cpp" )

set_target_properties(aceUnitTestEngineGtest PROPERTIES OUTPUT_NAME_DEBUG "UnitTestEngineGtest_cpp" )
set_target_properties(aceUnitTestEngineGtest PROPERTIES OUTPUT_NAME_RELEASE "UnitTestEngineGtest_cpp" )


# フラグ処理
if (MSVC)
	if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
		foreach (flag CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE)

			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()
endif()

# インストール先を決定
install(
	TARGETS aceCommon
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)

install(
	TARGETS aceCore
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib )

install(
	TARGETS aceCoreDLL
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin )

install(
	TARGETS aceEngine
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib )

install(
	TARGETS aceUnitTestGtest
	RUNTIME DESTINATION bin )

install(
	TARGETS aceUnitTestEngineGtest
	RUNTIME DESTINATION bin )

