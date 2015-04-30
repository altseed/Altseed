//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace ace
{
	struct Vector2DF;
	struct Vector2DI;
	struct Vector3DF;
	struct Vector4DF;
	struct Color;
	struct Matrix33;
	struct Matrix44;
	struct RectI;
	struct RectF;

	struct FCurveKeyframe;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

CPP_OBJECT( ace::CoreScene )
CPP_OBJECT( ace::CoreScene_Imp )
CPP_OBJECT( ace::CoreLayer )
CPP_OBJECT( ace::CoreLayer_Imp )

CPP_OBJECT( ace::CoreLayer2D )
CPP_OBJECT( ace::CoreObject2D )
CPP_OBJECT( ace::CoreTextureObject2D )
CPP_OBJECT( ace::CoreCameraObject2D )
CPP_OBJECT( ace::CoreTextObject2D )
CPP_OBJECT( ace::CoreEffectObject2D )
CPP_OBJECT( ace::CoreMapObject2D )
CPP_OBJECT( ace::CoreGeometryObject2D )

CPP_OBJECT( ace::CoreShape )
CPP_OBJECT( ace::CoreCircleShape )
CPP_OBJECT( ace::CoreArcShape )
CPP_OBJECT( ace::CoreLineShape )
CPP_OBJECT( ace::CoreTriangleShape )
CPP_OBJECT( ace::CoreRectangleShape )

CPP_OBJECT( ace::CoreObject3D )
CPP_OBJECT( ace::CoreModelObject3D )
CPP_OBJECT( ace::CoreDirectionalLightObject3D )
CPP_OBJECT( ace::CoreCameraObject3D )
CPP_OBJECT( ace::CoreLayer3D )

CPP_OBJECT( ace::CorePostEffect )
CPP_OBJECT( ace::CorePostEffect_Imp )

CPP_OBJECT( ace::CoreTransition )

CPP_OBJECT( ace::SoundSource )

CPP_OBJECT( ace::Texture )
CPP_OBJECT( ace::Texture2D )
CPP_OBJECT( ace::Texture2D_Imp )
CPP_OBJECT( ace::RenderTexture2D )
CPP_OBJECT( ace::RenderTexture2D_Imp )
CPP_OBJECT( ace::CubemapTexture )
CPP_OBJECT( ace::Material2D )
CPP_OBJECT( ace::Material3D )
CPP_OBJECT( ace::MaterialPropertyBlock )
CPP_OBJECT( ace::Shader2D )
CPP_OBJECT( ace::Shader2D_Imp )
CPP_OBJECT( ace::Shader3D )

CPP_OBJECT( ace::ImagePackage )

CPP_OBJECT( ace::Effect )

CPP_OBJECT( ace::Font )

CPP_OBJECT( ace::Chip2D )

CPP_OBJECT( ace::Mesh )
CPP_OBJECT( ace::Deformer )
CPP_OBJECT( ace::Model )
CPP_OBJECT( ace::MassModel )
CPP_OBJECT( ace::Terrain3D )

CPP_OBJECT( ace::KeyframeAnimation )
CPP_OBJECT( ace::AnimationSource )
CPP_OBJECT( ace::AnimationClip )

CPP_OBJECT( ace::Profiler )
CPP_OBJECT( ace::Profiler_Imp )
CPP_OBJECT( ace::LayerProfiler )
CPP_OBJECT( ace::LayerProfiler_Imp )
CPP_OBJECT( ace::Log )
CPP_OBJECT( ace::Log_Imp )
CPP_OBJECT( ace::File )
CPP_OBJECT( ace::File_Imp )
CPP_OBJECT( ace::StaticFile )
CPP_OBJECT( ace::StreamFile )

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
%newobject ace::Core::CreateCore;

%newobject ace::Graphics_Imp::CreateTexture2D_Imp;
%newobject ace::Graphics_Imp::CreateRenderTexture2D_Imp;
%newobject ace::Graphics_Imp::CreateCubemapTexture2D_;

%newobject ace::Graphics_Imp::CreateShader2D_Imp;
%newobject ace::Graphics_Imp::CreateMaterial2D_;
%newobject ace::Graphics_Imp::CreateMesh_;
%newobject ace::Graphics_Imp::CreateDeformer_;
%newobject ace::Graphics_Imp::CreateModel_;

%newobject ace::Graphics_Imp::CreateFont_;
%newobject ace::Graphics_Imp::CreateChip2D_;

%newobject ace::File_Imp::CreateStaticFile_;

%newobject ace::ObjectSystemFactory::CreateObject2D;
%newobject ace::ObjectSystemFactory::CreateTextureObject2D;
%newobject ace::ObjectSystemFactory::CreateFont;
%newobject ace::ObjectSystemFactory::CreateChip2D;
%newobject ace::ObjectSystemFactory::CreateLayer2D;

%newobject ace::ObjectSystemFactory::CreateArcShape;
%newobject ace::ObjectSystemFactory::CreateCircleShape;
%newobject ace::ObjectSystemFactory::CreateLineShape;
%newobject ace::ObjectSystemFactory::CreateRectangleShape;
%newobject ace::ObjectSystemFactory::CreateTriangleShape;
%newobject ace::ObjectSystemFactory::CreatePolygonShape;

%newobject ace::ObjectSystemFactory::CreateModelObject3D;
%newobject ace::ObjectSystemFactory::CreateCameraObject3D;
%newobject ace::ObjectSystemFactory::CreateDirectionalLightObject3D;
%newobject ace::ObjectSystemFactory::CreateLayer3D;

%newobject ace::ObjectSystemFactory::CreateScene;
%newobject ace::ObjectSystemFactory::CreatePostEffect;

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------


%include "ace_cpp/common/Graphics/ace.Graphics.Common.h"

%include "ace_cpp/core/ace.Core.Base.h"
%include "ace_cpp/core/ace.ReferenceObject.h"

%include "ace_cpp/core/Window/ace.Window.h"
%include "ace_cpp/core/Window/ace.Window_Imp.h"
%include "ace_cpp/core/Log/ace.Log.h"
%include "ace_cpp/core/Log/ace.Log_Imp.h"
%include "ace_cpp/core/Profiler/ace.Profiler.h"
%include "ace_cpp/core/Profiler/ace.Profiler_Imp.h"
%include "ace_cpp/core/Profiler/ace.LayerProfiler.h"
%include "ace_cpp/core/Profiler/ace.LayerProfiler_Imp.h"
%include "ace_cpp/core/Input/ace.MouseButtonState.h"
%include "ace_cpp/core/Input/ace.MiddleButton.h"
%include "ace_cpp/core/Input/ace.SideButton.h"
%include "ace_cpp/core/Input/ace.Mouse.h"
%include "ace_cpp/core/Input/ace.Mouse_Imp.h"
%include "ace_cpp/core/Input/ace.Keyboard.h"
%include "ace_cpp/core/Input/ace.Keyboard_Imp.h"
%include "ace_cpp/core/Input/ace.Joystick.h"
%include "ace_cpp/core/Input/ace.Joystick_Imp.h"
%include "ace_cpp/core/Input/ace.JoystickContainer.h"
%include "ace_cpp/core/Input/ace.JoystickContainer_Imp.h"
%include "ace_cpp/core/Input/ace.JoystickContainer_Imp.h"

%include "ace_cpp/core/IO/ace.File.h"
%include "ace_cpp/core/IO/ace.File_Imp.h"
%include "ace_cpp/core/IO/ace.StaticFile.h"
%include "ace_cpp/core/IO/ace.StreamFile.h"

%include "ace_cpp/core/Sound/ace.SoundSource.h"
%include "ace_cpp/core/Sound/ace.Sound.h"

%include "ace_cpp/core/Graphics/Resource/ace.Shader2D.h"
%include "ace_cpp/core/Graphics/Resource/ace.Shader2D_Imp.h"

%include "ace_cpp/core/Graphics/Resource/ace.Material2D.h"

%include "ace_cpp/core/Graphics/Resource/ace.Shader3D.h"
%include "ace_cpp/core/Graphics/Resource/ace.Material3D.h"

%include "ace_cpp/core/Graphics/Resource/ace.MaterialPropertyBlock.h"

%include "ace_cpp/core/Graphics/Resource/ace.Texture.h"
%include "ace_cpp/core/Graphics/Resource/ace.Texture2D.h"
%include "ace_cpp/core/Graphics/Resource/ace.Texture2D_Imp.h"
%include "ace_cpp/core/Graphics/Resource/ace.RenderTexture2D.h"
%include "ace_cpp/core/Graphics/Resource/ace.RenderTexture2D_Imp.h"
%include "ace_cpp/core/Graphics/Resource/ace.CubemapTexture.h"

%include "ace_cpp/core/Graphics/Resource/ace.ImagePackage.h"

%include "ace_cpp/core/Graphics/Resource/ace.Effect.h"

%include "ace_cpp/core/Graphics/Resource/ace.Font.h"

%include "ace_cpp/core/Graphics/Resource/ace.Chip2D.h"

%include "ace_cpp/core/Graphics/3D/Resource/ace.Deformer.h"
%include "ace_cpp/core/Graphics/3D/Resource/ace.Mesh.h"
%include "ace_cpp/core/Graphics/3D/Resource/ace.Model.h"
%include "ace_cpp/core/Graphics/3D/Resource/ace.MassModel.h"
%include "ace_cpp/core/Graphics/3D/Resource/ace.Terrain3D.h"

%include "ace_cpp/core/Graphics/3D/Resource/Animation/ace.KeyframeAnimation.h"
%include "ace_cpp/core/Graphics/3D/Resource/Animation/ace.AnimationSource.h"
%include "ace_cpp/core/Graphics/3D/Resource/Animation/ace.AnimationClip.h"
%include "ace_cpp/core/Graphics/3D/Resource/Animation/ace.AnimationSystem.h"

%include "ace_cpp/core/Graphics/ace.Graphics.h"
%include "ace_cpp/core/Graphics/ace.Graphics_Imp.h"

%include "ace_cpp/core/Shape/ace.ShapeType.h"
%include "ace_cpp/core/Shape/ace.CoreShape.h"
%include "ace_cpp/core/Shape/ace.CoreTriangleShape.h"
%include "ace_cpp/core/Shape/ace.CoreCircleShape.h"
%include "ace_cpp/core/Shape/ace.CoreArcShape.h"
%include "ace_cpp/core/Shape/ace.CoreLineShape.h"
%include "ace_cpp/core/Shape/ace.CoreRectangleShape.h"
%include "ace_cpp/core/Shape/ace.CorePolygonShape.h"

%include "ace_cpp/core/ObjectSystem/ace.ChildMode.h"
%include "ace_cpp/core/ObjectSystem/2D/ace.CoreObject2D.h"
%include "ace_cpp/core/ObjectSystem/2D/ace.CoreTextureObject2D.h"
%include "ace_cpp/core/ObjectSystem/2D/ace.CoreCameraObject2D.h"
%include "ace_cpp/core/ObjectSystem/2D/ace.CoreTextObject2D.h"
%include "ace_cpp/core/ObjectSystem/2D/ace.CoreEffectObject2D.h"
%include "ace_cpp/core/ObjectSystem/2D/ace.CoreMapObject2D.h"
%include "ace_cpp/core/ObjectSystem/2D/ace.CoreGeometryObject2D.h"

%include "ace_cpp/core/ObjectSystem/ace.CoreLayer.h"

%include "ace_cpp/core/ObjectSystem/2D/ace.CoreLayer2D.h"

%include "ace_cpp/core/ObjectSystem/3D/ace.CoreObject3D.h"
%include "ace_cpp/core/ObjectSystem/3D/ace.CoreModelObject3D.h"
%include "ace_cpp/core/ObjectSystem/3D/ace.CoreDirectionalLightObject3D.h"
%include "ace_cpp/core/ObjectSystem/3D/ace.CoreCameraObject3D.h"
%include "ace_cpp/core/ObjectSystem/3D/ace.CoreEffectObject3D.h"
%include "ace_cpp/core/ObjectSystem/3D/ace.CoreMassModelObject3D.h"
%include "ace_cpp/core/ObjectSystem/3D/ace.CoreTerrainObject3D.h"

%include "ace_cpp/core/ObjectSystem/3D/ace.CoreLayer3D.h"

%include "ace_cpp/core/ObjectSystem/ace.CoreScene.h"
%include "ace_cpp/core/ObjectSystem/PostEffect/ace.CorePostEffect.h"
%include "ace_cpp/core/ObjectSystem/ace.ObjectSystemFactory.h"

%include "ace_cpp/core/ObjectSystem/Transition/ace.CoreTransition.h"

%include "ace_cpp/core/Core/ace.Core.h"
%include "ace_cpp/core/Core/ace.Core_Imp.h"

%include "ace_cpp/core/Core/ace.CreateCore.h"

%include "ace_cpp/core/ace.Accessor.h"
%include "ace_cpp/core/ace.StructTranslator.h"