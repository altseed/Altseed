//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace asd
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

CPP_OBJECT( asd::CoreScene )
CPP_OBJECT( asd::CoreScene_Imp )
CPP_OBJECT( asd::CoreLayer )
CPP_OBJECT( asd::CoreLayer_Imp )


CPP_OBJECT( asd::CoreChip2D )
CPP_OBJECT( asd::CoreLayer2D )
CPP_OBJECT( asd::CoreObject2D )
CPP_OBJECT( asd::CoreDrawnObject2D )
CPP_OBJECT( asd::CoreTextureObject2D )
CPP_OBJECT( asd::CoreCameraObject2D )
CPP_OBJECT( asd::CoreTextObject2D )
CPP_OBJECT( asd::CoreEffectObject2D )
CPP_OBJECT( asd::CoreMapObject2D )
CPP_OBJECT( asd::CoreGeometryObject2D )

CPP_OBJECT( asd::CoreShape )
CPP_OBJECT( asd::CoreCircleShape )
CPP_OBJECT( asd::CoreArcShape )
CPP_OBJECT( asd::CoreLineShape )
CPP_OBJECT( asd::CoreTriangleShape )
CPP_OBJECT( asd::CoreRectangleShape )

CPP_OBJECT( asd::CoreCollision2D )
CPP_OBJECT( asd::CoreCircleCollider )
CPP_OBJECT( asd::CoreLineCollider )
CPP_OBJECT( asd::CoreRectangleCollider )
CPP_OBJECT( asd::CorePolygonCollider )

CPP_OBJECT( asd::CoreObject3D )
CPP_OBJECT( asd::CoreModelObject3D )
CPP_OBJECT( asd::CoreDirectionalLightObject3D )
CPP_OBJECT( asd::CoreCameraObject3D )
CPP_OBJECT( asd::CoreLayer3D )

CPP_OBJECT( asd::CorePostEffect )
CPP_OBJECT( asd::CorePostEffect_Imp )

CPP_OBJECT( asd::CoreTransition )

CPP_OBJECT( asd::Cursor )

CPP_OBJECT( asd::SoundSource )

CPP_OBJECT( asd::Texture )
CPP_OBJECT( asd::Texture2D )
CPP_OBJECT( asd::Texture2D_Imp )
CPP_OBJECT( asd::RenderTexture2D )
CPP_OBJECT( asd::RenderTexture2D_Imp )
CPP_OBJECT( asd::CubemapTexture )
CPP_OBJECT( asd::Material2D )
CPP_OBJECT( asd::Material3D )
CPP_OBJECT( asd::MaterialPropertyBlock )
CPP_OBJECT( asd::Shader2D )
CPP_OBJECT( asd::Shader2D_Imp )
CPP_OBJECT( asd::Shader3D )

CPP_OBJECT( asd::ImagePackage )

CPP_OBJECT( asd::Effect )

CPP_OBJECT( asd::Font )

CPP_OBJECT( asd::Mesh )
CPP_OBJECT( asd::Deformer )
CPP_OBJECT( asd::Model )
CPP_OBJECT( asd::MassModel )
CPP_OBJECT( asd::Terrain3D )

CPP_OBJECT( asd::KeyframeAnimation )
CPP_OBJECT( asd::AnimationSource )
CPP_OBJECT( asd::AnimationClip )

CPP_OBJECT( asd::Profiler )
CPP_OBJECT( asd::LayerProfiler )
CPP_OBJECT( asd::LayerProfiler_Imp )
CPP_OBJECT( asd::Log )

CPP_OBJECT( asd::File )
CPP_OBJECT( asd::StaticFile )
CPP_OBJECT( asd::StreamFile )

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
%newobject asd::Core::CreateCore;

%newobject asd::Graphics_Imp::CreateTexture2D_Imp;
%newobject asd::Graphics_Imp::CreateRenderTexture2D_Imp;
%newobject asd::Graphics_Imp::CreateCubemapTexture2D_;

%newobject asd::Graphics_Imp::CreateShader2D_Imp;
%newobject asd::Graphics_Imp::CreateMaterial2D_;
%newobject asd::Graphics_Imp::CreateMesh_;
%newobject asd::Graphics_Imp::CreateDeformer_;
%newobject asd::Graphics_Imp::CreateModel_;

%newobject asd::Graphics_Imp::CreateFont_;
%newobject asd::Graphics_Imp::CreateChip2D_;

%newobject asd::File_Imp::CreateStaticFile_;

%newobject asd::ObjectSystemFactory::CreateObject2D;
%newobject asd::ObjectSystemFactory::CreateTextureObject2D;
%newobject asd::ObjectSystemFactory::CreateFont;
%newobject asd::ObjectSystemFactory::CreateChip2D;
%newobject asd::ObjectSystemFactory::CreateLayer2D;

%newobject asd::ObjectSystemFactory::CreateArcShape;
%newobject asd::ObjectSystemFactory::CreateCircleShape;
%newobject asd::ObjectSystemFactory::CreateLineShape;
%newobject asd::ObjectSystemFactory::CreateRectangleShape;
%newobject asd::ObjectSystemFactory::CreateTriangleShape;
%newobject asd::ObjectSystemFactory::CreatePolygonShape;

%newobject asd::ObjectSystemFactory::CreateCircleCollider;
%newobject asd::ObjectSystemFactory::CreateLineCollider;
%newobject asd::ObjectSystemFactory::CreateRectangleCollider;
%newobject asd::ObjectSystemFactory::CreatePolygonCollider;

%newobject asd::ObjectSystemFactory::CreateModelObject3D;
%newobject asd::ObjectSystemFactory::CreateCameraObject3D;
%newobject asd::ObjectSystemFactory::CreateDirectionalLightObject3D;
%newobject asd::ObjectSystemFactory::CreateLayer3D;

%newobject asd::ObjectSystemFactory::CreateScene;
%newobject asd::ObjectSystemFactory::CreatePostEffect;

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------


%include "asd_cpp/common/Graphics/asd.Graphics.Common.h"

%include "asd_cpp/core/asd.Core.Base.h"
%include "asd_cpp/core/asd.ReferenceObject.h"

%include "asd_cpp/core/Window/asd.Cursor.h"
%include "asd_cpp/core/Window/asd.Window.h"
%include "asd_cpp/core/Window/asd.Window_Imp.h"

%include "asd_cpp/core/Log/asd.Log.h"

%include "asd_cpp/core/Profiler/asd.Profiler.h"

%include "asd_cpp/core/Profiler/asd.LayerProfiler.h"
%include "asd_cpp/core/Profiler/asd.LayerProfiler_Imp.h"

%include "asd_cpp/core/Input/asd.MouseButtonState.h"
%include "asd_cpp/core/Input/asd.MiddleButton.h"
%include "asd_cpp/core/Input/asd.SideButton.h"
%include "asd_cpp/core/Input/asd.Mouse.h"
%include "asd_cpp/core/Input/asd.Keyboard.h"
%include "asd_cpp/core/Input/asd.Joystick.h"
%include "asd_cpp/core/Input/asd.JoystickContainer.h"

%include "asd_cpp/core/IO/asd.File.h"
%include "asd_cpp/core/IO/asd.StaticFile.h"
%include "asd_cpp/core/IO/asd.StreamFile.h"

%include "asd_cpp/core/Sound/asd.SoundSource.h"
%include "asd_cpp/core/Sound/asd.Sound.h"

%include "asd_cpp/core/Graphics/Resource/asd.Shader2D.h"

%include "asd_cpp/core/Graphics/Resource/asd.Material2D.h"

%include "asd_cpp/core/Graphics/Resource/asd.Shader3D.h"
%include "asd_cpp/core/Graphics/Resource/asd.Material3D.h"

%include "asd_cpp/core/Graphics/Resource/asd.MaterialPropertyBlock.h"

%include "asd_cpp/core/Graphics/Resource/asd.Texture.h"
%include "asd_cpp/core/Graphics/Resource/asd.Texture2D.h"
%include "asd_cpp/core/Graphics/Resource/asd.Texture2D_Imp.h"
%include "asd_cpp/core/Graphics/Resource/asd.RenderTexture2D.h"
%include "asd_cpp/core/Graphics/Resource/asd.RenderTexture2D_Imp.h"
%include "asd_cpp/core/Graphics/Resource/asd.CubemapTexture.h"

%include "asd_cpp/core/Graphics/Resource/asd.ImagePackage.h"

%include "asd_cpp/core/Graphics/Resource/asd.Effect.h"

%include "asd_cpp/core/Graphics/Resource/asd.Font.h"

%include "asd_cpp/core/Graphics/3D/Resource/asd.Deformer.h"
%include "asd_cpp/core/Graphics/3D/Resource/asd.Mesh.h"
%include "asd_cpp/core/Graphics/3D/Resource/asd.Model.h"
%include "asd_cpp/core/Graphics/3D/Resource/asd.MassModel.h"
%include "asd_cpp/core/Graphics/3D/Resource/asd.Terrain3D.h"

%include "asd_cpp/core/Graphics/3D/Resource/Animation/asd.KeyframeAnimation.h"
%include "asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationSource.h"
%include "asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationClip.h"
%include "asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationSystem.h"

%include "asd_cpp/core/Graphics/asd.Graphics.h"
%include "asd_cpp/core/Graphics/asd.Graphics_Imp.h"

%include "asd_cpp/core/Shape/asd.ShapeType.h"
%include "asd_cpp/core/Shape/asd.CoreShape.h"
%include "asd_cpp/core/Shape/asd.CoreTriangleShape.h"
%include "asd_cpp/core/Shape/asd.CoreCircleShape.h"
%include "asd_cpp/core/Shape/asd.CoreArcShape.h"
%include "asd_cpp/core/Shape/asd.CoreLineShape.h"
%include "asd_cpp/core/Shape/asd.CoreRectangleShape.h"
%include "asd_cpp/core/Shape/asd.CorePolygonShape.h"


%include "asd_cpp/core/Collision/2D/asd.Collider2DType.h"
%include "asd_cpp/core/Collision/2D/asd.CoreCollision2D.h"
%include "asd_cpp/core/Collision/2D/asd.CoreCollision2DEvent.h"
%include "asd_cpp/core/Collision/2D/asd.CoreCollider2D.h"
%include "asd_cpp/core/Collision/2D/asd.CoreCircleCollider.h"
%include "asd_cpp/core/Collision/2D/asd.CoreLineCollider.h"
%include "asd_cpp/core/Collision/2D/asd.CoreRectangleCollider.h"
%include "asd_cpp/core/Collision/2D/asd.CorePolygonCollider.h"

%include "asd_cpp/core/ObjectSystem/asd.ChildManagementMode.h"
%include "asd_cpp/core/ObjectSystem/asd.ChildTransformingMode.h"
%include "asd_cpp/core/ObjectSystem/asd.ChildDrawingMode.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreChip2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreObject2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreDrawnObject2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreTextureObject2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreCameraObject2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreTextObject2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreEffectObject2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreMapObject2D.h"
%include "asd_cpp/core/ObjectSystem/2D/asd.CoreGeometryObject2D.h"

%include "asd_cpp/core/ObjectSystem/asd.CoreLayer.h"

%include "asd_cpp/core/ObjectSystem/2D/asd.CoreLayer2D.h"

%include "asd_cpp/core/ObjectSystem/3D/asd.CoreObject3D.h"
%include "asd_cpp/core/ObjectSystem/3D/asd.CoreModelObject3D.h"
%include "asd_cpp/core/ObjectSystem/3D/asd.CoreDirectionalLightObject3D.h"
%include "asd_cpp/core/ObjectSystem/3D/asd.CoreCameraObject3D.h"
%include "asd_cpp/core/ObjectSystem/3D/asd.CoreEffectObject3D.h"
%include "asd_cpp/core/ObjectSystem/3D/asd.CoreMassModelObject3D.h"
%include "asd_cpp/core/ObjectSystem/3D/asd.CoreTerrainObject3D.h"

%include "asd_cpp/core/ObjectSystem/3D/asd.CoreLayer3D.h"

%include "asd_cpp/core/ObjectSystem/asd.CoreScene.h"
%include "asd_cpp/core/ObjectSystem/PostEffect/asd.CorePostEffect.h"
%include "asd_cpp/core/ObjectSystem/asd.ObjectSystemFactory.h"

%include "asd_cpp/core/ObjectSystem/Transition/asd.CoreTransition.h"

%include "asd_cpp/core/Core/asd.Core.h"
%include "asd_cpp/core/Core/asd.Core_Imp.h"

%include "asd_cpp/core/Core/asd.CreateCore.h"

%include "asd_cpp/core/asd.Accessor.h"
%include "asd_cpp/core/asd.StructTranslator.h"