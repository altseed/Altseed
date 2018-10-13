import re
import codecs
import os
import os.path

def cd(path):
	os.chdir(path)

def cdToScript():
	cd(os.path.dirname(os.path.abspath(__file__)))

class CreateHeader:
	def __init__(self):
		self.lines = []

	def append(self,s):
		self.lines.append(s)

	def readLines(self,path):
		f = codecs.open(path, 'r','utf-8_sig')
		line = f.readline()
		while line:
			if re.search('include( ?)\"', line) != None:
				line = f.readline()
				continue;

			if re.search('#pragma once', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <asd.', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <Graphics/asd.', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <Math/asd.', line) != None:
				line = f.readline()
				continue;

			self.lines.append(line)
			line = f.readline()
		self.lines.append('\r\n')
		f.close

	def output(self,path):
		f =  codecs.open(path, 'w','utf-8_sig')
		for line in self.lines:
			f.write(line)
		f.close()

cdToScript()

engineHeader = CreateHeader()
engineHeader.append('#pragma once\r\n')
engineHeader.append('#define __ALTSEED_HEADER__\r\n')
engineHeader.readLines("asd_cpp/common/asd.common.Base.h")
engineHeader.readLines("asd_cpp/engine/asd.Engine.Base.h")

# primitive
engineHeader.readLines("asd_cpp/common/Math/asd.Vector2DI.h")
engineHeader.readLines("asd_cpp/common/Math/asd.Vector2DF.h")
engineHeader.readLines("asd_cpp/common/Math/asd.Vector3DF.h")
engineHeader.readLines("asd_cpp/common/Math/asd.Vector4DF.h")
engineHeader.readLines("asd_cpp/common/Math/asd.Matrix33.h")
engineHeader.readLines("asd_cpp/common/Math/asd.Matrix44.h")
engineHeader.readLines("asd_cpp/common/Math/asd.RectI.h")
engineHeader.readLines("asd_cpp/common/Math/asd.RectF.h")

engineHeader.readLines("asd_cpp/common/Utility/asd.GetTypeName.h")
engineHeader.readLines("asd_cpp/common/Utility/asd.PointerArray.h")
engineHeader.readLines("asd_cpp/common/Utility/asd.Timer.h")
engineHeader.readLines("asd_cpp/common/Utility/asd.TypeErasureCopy.h")

engineHeader.readLines("asd_cpp/common/Graphics/asd.Graphics.Common.h")
engineHeader.readLines("asd_cpp/common/Graphics/asd.Color.h")

engineHeader.readLines("asd_cpp/common/Utility/asd.BinaryReader.h")
engineHeader.readLines("asd_cpp/common/Utility/asd.BinaryWriter.h")

# core bases
engineHeader.readLines("asd_cpp/core/asd.Core.Base.h")
engineHeader.readLines("asd_cpp/core/asd.ReferenceObject.h")

# wrapper
# since graphics referes a class in the wrapper, so it appends previously
engineHeader.readLines("asd_cpp/engine/asd.WrapperLib.h")

engineHeader.readLines("asd_cpp/core/Input/asd.MouseButtonState.h")
engineHeader.readLines("asd_cpp/core/Input/asd.SideButton.h")
engineHeader.readLines("asd_cpp/core/Input/asd.MiddleButton.h")
engineHeader.readLines("asd_cpp/core/Input/asd.Mouse.h")
engineHeader.readLines("asd_cpp/core/Input/asd.Keyboard.h")

engineHeader.readLines("asd_cpp/core/Input/asd.Joystick.h")
engineHeader.readLines("asd_cpp/core/Input/asd.JoystickContainer.h")

engineHeader.readLines("asd_cpp/core/Log/asd.Log.h")
engineHeader.readLines("asd_cpp/core/Profiler/asd.Profiler.h")
        
#engineHeader.readLines("asd_cpp/core/IO/asd.File.h")
#engineHeader.readLines("asd_cpp/core/IO/asd.StaticFile.h")
#engineHeader.readLines("asd_cpp/core/IO/asd.StreamFile.h")

#engineHeader.readLines("asd_cpp/core/Sound/asd.SoundSource.h")
#engineHeader.readLines("asd_cpp/core/Sound/asd.Sound.h")

engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Texture.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Texture2D.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.RenderTexture2D.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.CubemapTexture.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Shader2D.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Shader3D.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Material2D.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Material3D.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.MaterialPropertyBlock.h")
engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Font.h")

engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.ImagePackage.h")

engineHeader.readLines("asd_cpp/core/Graphics/Resource/asd.Effect.h")


engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Deformer.h")
engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Mesh.h")
engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Model.h")
engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/asd.MassModel.h")
engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Terrain3D.h")

engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.KeyframeAnimation.h")
engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationSource.h")
engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationClip.h")
engineHeader.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationSystem.h")

engineHeader.readLines("asd_cpp/core/Graphics/asd.Graphics.h")

engineHeader.readLines("asd_cpp/core/Shape/asd.ShapeType.h")

engineHeader.readLines("asd_cpp/core/Window/asd.Cursor.h")

engineHeader.readLines("asd_cpp/core/Core/asd.Core.h")

engineHeader.readLines("asd_cpp/engine/ObjectSystem/PostEffect/asd.PostEffect.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/PostEffect/asd.PostEffectGrayScale.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/PostEffect/asd.PostEffectSepia.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/PostEffect/asd.PostEffectGaussianBlur.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/PostEffect/asd.PostEffectLightBloom.h")

engineHeader.readLines("asd_cpp/engine/ObjectSystem/Transition/asd.Transition.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/Transition/asd.TransitionFade.h")

engineHeader.readLines("asd_cpp/engine/ObjectSystem/Registration/asd.ICommitable.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/Registration/asd.IComponentRegisterable.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/Registration/asd.IImmediateComponentManager.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/Registration/asd.IImmediateObjectManager.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/Registration/asd.IImmediatelyDisposable.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/Registration/asd.IObjectRegisterable.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/Registration/asd.RegistrationCommand.h")

#engineHeader.readLines('asd_cpp/engine/ObjectSystem/Component/asd.ComponentManager.h')
engineHeader.readLines('asd_cpp/engine/ObjectSystem/Component/asd.Object2DComponent.h')
engineHeader.readLines('asd_cpp/engine/ObjectSystem/Component/asd.Layer2DComponent.h')
engineHeader.readLines('asd_cpp/engine/ObjectSystem/Component/asd.SceneComponent.h')

engineHeader.readLines("asd_cpp/engine/ObjectSystem/asd.Layer.h")

engineHeader.readLines("asd_cpp/engine/Shape/asd.Shape.h")
engineHeader.readLines("asd_cpp/engine/Shape/asd.TriangleShape.h")
engineHeader.readLines("asd_cpp/engine/Shape/asd.CircleShape.h")
engineHeader.readLines("asd_cpp/engine/Shape/asd.ArcShape.h")
engineHeader.readLines("asd_cpp/engine/Shape/asd.RectangleShape.h")
engineHeader.readLines("asd_cpp/engine/Shape/asd.LineShape.h")
engineHeader.readLines("asd_cpp/engine/Shape/asd.PolygonShape.h")

engineHeader.readLines("asd_cpp/core/Collision/2D/asd.Collider2DType.h")
engineHeader.readLines("asd_cpp/core/Collision/2D/asd.CoreCollision2DEvent.h")
engineHeader.readLines("asd_cpp/engine/Collision/2D/asd.Collider2D.h")
engineHeader.readLines("asd_cpp/engine/Collision/2D/asd.Collision2DInfo.h")
engineHeader.readLines("asd_cpp/engine/Collision/2D/asd.CircleCollider.h")
engineHeader.readLines("asd_cpp/engine/Collision/2D/asd.LineCollider.h")
engineHeader.readLines("asd_cpp/engine/Collision/2D/asd.RectangleCollider.h")
engineHeader.readLines("asd_cpp/engine/Collision/2D/asd.PolygonCollider.h")

engineHeader.readLines("asd_cpp/core/ObjectSystem/asd.ChildManagementMode.h")
engineHeader.readLines("asd_cpp/core/ObjectSystem/asd.ChildTransformingMode.h")
engineHeader.readLines("asd_cpp/core/ObjectSystem/asd.ChildDrawingMode.h")

engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.Chip2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.Object2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.DrawnObject2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.TextureObject2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.CameraObject2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.TextObject2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.EffectObject2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.MapObject2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.GeometryObject2D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/2D/asd.Layer2D.h")

engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.Object3D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.CameraObject3D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.DirectionalLightObject3D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.EffectObject3D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.ModelObject3D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.MassModelObject3D.h")
engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.TerrainObject3D.h")

engineHeader.readLines("asd_cpp/engine/ObjectSystem/3D/asd.Layer3D.h")

engineHeader.readLines("asd_cpp/engine/ObjectSystem/asd.Scene.h")

engineHeader.readLines("asd_cpp/engine/asd.Engine.h")

engineHeader.output('include/Altseed.h')
