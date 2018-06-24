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

fromCoreToEngine = CreateHeader()
fromCoreToEngine.append('#pragma once')
fromCoreToEngine.append('\n#include <stdint.h>')
fromCoreToEngine.append('\n#include <asd.common.Base.h>')


fromCoreToEngine.readLines("asd_cpp/common/Math/asd.Vector2DI.h")
fromCoreToEngine.readLines("asd_cpp/common/Math/asd.Vector2DF.h")
fromCoreToEngine.readLines("asd_cpp/common/Math/asd.RectI.h")
fromCoreToEngine.readLines("asd_cpp/common/Math/asd.RectF.h")
fromCoreToEngine.append('namespace ace{ struct Vector3DF; }')
fromCoreToEngine.readLines("asd_cpp/common/Math/asd.Matrix44.h")
fromCoreToEngine.readLines("asd_cpp/common/Math/asd.Vector3DF.h")
fromCoreToEngine.readLines("asd_cpp/common/Math/asd.Vector4DF.h")
fromCoreToEngine.readLines("asd_cpp/common/Math/asd.Matrix33.h")

fromCoreToEngine.readLines("asd_cpp/common/Graphics/asd.Graphics.Common.h")
fromCoreToEngine.readLines("asd_cpp/common/Graphics/asd.Color.h")

fromCoreToEngine.readLines("asd_cpp/core/asd.Core.Base.h")
fromCoreToEngine.readLines("asd_cpp/core/asd.ReferenceObject.h")

fromCoreToEngine.readLines("asd_cpp/core/Input/asd.MouseButtonState.h")
fromCoreToEngine.readLines("asd_cpp/core/Input/asd.SideButton.h")
fromCoreToEngine.readLines("asd_cpp/core/Input/asd.MiddleButton.h")
fromCoreToEngine.readLines("asd_cpp/core/Input/asd.Mouse.h")
fromCoreToEngine.readLines("asd_cpp/core/Input/asd.Keyboard.h")
fromCoreToEngine.readLines("asd_cpp/core/Input/asd.Joystick.h")
fromCoreToEngine.readLines("asd_cpp/core/Input/asd.JoystickContainer.h")
fromCoreToEngine.readLines("asd_cpp/core/Log/asd.Log.h")
fromCoreToEngine.readLines("asd_cpp/core/Profiler/asd.Profiler.h")
fromCoreToEngine.readLines("asd_cpp/core/Profiler/asd.LayerProfiler.h")
                                                                       
#fromCoreToEngine.readLines("asd_cpp/core/IO/asd.File.h")
#fromCoreToEngine.readLines("asd_cpp/core/IO/asd.StaticFile.h")
#fromCoreToEngine.readLines("asd_cpp/core/IO/asd.StreamFile.h")

#fromCoreToEngine.readLines("asd_cpp/core/Sound/asd.SoundSource.h")
#fromCoreToEngine.readLines("asd_cpp/core/Sound/asd.Sound.h")

fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Texture.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Texture2D.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.RenderTexture2D.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.CubemapTexture.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Shader2D.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Shader3D.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Material2D.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Material3D.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.MaterialPropertyBlock.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Font.h")

fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.ImagePackage.h")


fromCoreToEngine.readLines("asd_cpp/core/Graphics/Resource/asd.Effect.h")

fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Deformer.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Mesh.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Model.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/asd.MassModel.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/asd.Terrain3D.h")

fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.KeyframeAnimation.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationSource.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationClip.h")
fromCoreToEngine.readLines("asd_cpp/core/Graphics/3D/Resource/Animation/asd.AnimationSystem.h")

fromCoreToEngine.readLines("asd_cpp/core/Graphics/asd.Graphics.h")

fromCoreToEngine.readLines("asd_cpp/core/Window/asd.Cursor.h")

fromCoreToEngine.readLines("asd_cpp/core/Core/asd.Core.h")
fromCoreToEngine.readLines("asd_cpp/core/Core/asd.CreateCore.h")


fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.ShapeType.h")
fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.CoreShape.h")
fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.CoreTriangleShape.h")
fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.CoreCircleShape.h")
fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.CoreArcShape.h")
fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.CoreRectangleShape.h")
fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.CoreLineShape.h")
fromCoreToEngine.readLines("asd_cpp/core/Shape/asd.CorePolygonShape.h")

fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/asd.ChildManagementMode.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/asd.ChildTransformingMode.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/asd.ChildDrawingMode.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/asd.CoreLayer.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/asd.CoreScene.h')

fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.Collider2DType.h")
fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.CoreCollision2D.h")
fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.CoreCollision2DEvent.h")
fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.CoreCollider2D.h")
fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.CoreCircleCollider.h")
fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.CoreLineCollider.h")
fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.CoreRectangleCollider.h")
fromCoreToEngine.readLines("asd_cpp/core/Collision/2D/asd.CorePolygonCollider.h")

fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreObject2D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreDrawnObject2D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreTextureObject2D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreCameraObject2D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreTextObject2D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreEffectObject2D.h')
fromCoreToEngine.readLines("asd_cpp/core/ObjectSystem/2D/asd.CoreChip2D.h")
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreMapObject2D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreGeometryObject2D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/2D/asd.CoreLayer2D.h')

fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/3D/asd.CoreObject3D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/3D/asd.CoreModelObject3D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/3D/asd.CoreDirectionalLightObject3D.h')
fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/3D/asd.CoreCameraObject3D.h')
fromCoreToEngine.readLines("asd_cpp/core/ObjectSystem/3D/asd.CoreEffectObject3D.h")
fromCoreToEngine.readLines("asd_cpp/core/ObjectSystem/3D/asd.CoreMassModelObject3D.h")
fromCoreToEngine.readLines("asd_cpp/core/ObjectSystem/3D/asd.CoreTerrainObject3D.h")

fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/3D/asd.CoreLayer3D.h')

fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/PostEffect/asd.CorePostEffect.h')

fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/Transition/asd.CoreTransition.h')

fromCoreToEngine.readLines('asd_cpp/core/asd.WrapperDLL.h')

fromCoreToEngine.readLines('asd_cpp/core/ObjectSystem/asd.ObjectSystemFactory.h')

fromCoreToEngine.output('asd_cpp/engine/asd.CoreToEngine.h')
