import re
import codecs
import os
import os.path

def cd(path):
	os.chdir(path)

def cdToScript():
	cd(os.path.dirname(__file__))

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

			if re.search('#include <ace.', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <Graphics/ace.', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <Math/ace.', line) != None:
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
engineHeader.append('#pragma once')
engineHeader.readLines("ace_cpp/common/ace.common.Base.h")
engineHeader.readLines("ace_cpp/engine/ace.Engine.Base.h")

engineHeader.readLines("ace_cpp/common/Math/ace.Vector2DI.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Vector2DF.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Vector3DF.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Vector4DF.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Matrix33.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Matrix44.h")
engineHeader.readLines("ace_cpp/common/Math/ace.RectI.h")
engineHeader.readLines("ace_cpp/common/Math/ace.RectF.h")

engineHeader.readLines("ace_cpp/common/Utility/ace.GetTypeName.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.PointerArray.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.Timer.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.TypeErasureCopy.h")

engineHeader.readLines("ace_cpp/common/Graphics/ace.Graphics.Common.h")
engineHeader.readLines("ace_cpp/common/Graphics/ace.Color.h")

engineHeader.readLines("ace_cpp/common/Utility/ace.BinaryReader.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.BinaryWriter.h")

engineHeader.readLines("ace_cpp/core/ace.Core.Base.h")
engineHeader.readLines("ace_cpp/core/ace.ReferenceObject.h")

engineHeader.readLines("ace_cpp/core/Input/ace.MouseButtonState.h")
engineHeader.readLines("ace_cpp/core/Input/ace.SideButton.h")
engineHeader.readLines("ace_cpp/core/Input/ace.MiddleButton.h")
engineHeader.readLines("ace_cpp/core/Input/ace.Mouse.h")
engineHeader.readLines("ace_cpp/core/Input/ace.Keyboard.h")

engineHeader.readLines("ace_cpp/core/Input/ace.Joystick.h")
engineHeader.readLines("ace_cpp/core/Input/ace.JoystickContainer.h")

engineHeader.readLines("ace_cpp/core/Log/ace.Log.h")
engineHeader.readLines("ace_cpp/core/Profiler/ace.Profiler.h")
        
engineHeader.readLines("ace_cpp/core/IO/ace.File.h")
engineHeader.readLines("ace_cpp/core/IO/ace.Path.h")
engineHeader.readLines("ace_cpp/core/IO/ace.StaticFile.h")
engineHeader.readLines("ace_cpp/core/IO/ace.StreamFile.h")

engineHeader.readLines("ace_cpp/core/Sound/ace.SoundSource.h")
engineHeader.readLines("ace_cpp/core/Sound/ace.Sound.h")

engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Texture.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Texture2D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.RenderTexture2D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.CubemapTexture.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Shader2D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Shader3D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Material2D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Material3D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.MaterialPropertyBlock.h")
engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Font.h")

engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.ImagePackage.h")

engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Effect.h")

engineHeader.readLines("ace_cpp/core/Graphics/Resource/ace.Chip2D.h")

engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/ace.Deformer.h")
engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/ace.Mesh.h")
engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/ace.Model.h")
engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/ace.MassModel.h")
engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/ace.Terrain3D.h")

engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/Animation/ace.KeyframeAnimation.h")
engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/Animation/ace.AnimationSource.h")
engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/Animation/ace.AnimationClip.h")
engineHeader.readLines("ace_cpp/core/Graphics/3D/Resource/Animation/ace.AnimationSystem.h")

engineHeader.readLines("ace_cpp/core/Graphics/ace.Graphics.h")

engineHeader.readLines("ace_cpp/core/Shape/ace.ShapeType.h")

engineHeader.readLines("ace_cpp/core/Core/ace.Core.h")

engineHeader.readLines("ace_cpp/core/ObjectSystem/ace.ChildMode.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffect.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectGrayScale.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectSepia.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectGaussianBlur.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectLightBloom.h")

engineHeader.readLines("ace_cpp/engine/ObjectSystem/Transition/ace.Transition.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/Transition/ace.TransitionFade.h")

engineHeader.readLines('ace_cpp/engine/ObjectSystem/Component/ace.ComponentManager.h')
engineHeader.readLines('ace_cpp/engine/ObjectSystem/Component/ace.Object2DComponent.h')
engineHeader.readLines('ace_cpp/engine/ObjectSystem/Component/ace.Layer2DComponent.h')
engineHeader.readLines('ace_cpp/engine/ObjectSystem/Component/ace.SceneComponent.h')

engineHeader.readLines("ace_cpp/engine/ObjectSystem/ace.Layer.h")

engineHeader.readLines("ace_cpp/engine/ObjectSystem/ace.ContentsManager.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.Object2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.TextureObject2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.CameraObject2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.TextObject2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.EffectObject2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.MapObject2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.Layer2D.h")

engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.Object3D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.CameraObject3D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.DirectionalLightObject3D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.EffectObject3D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.ModelObject3D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.MassModelObject3D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.TerrainObject3D.h")

engineHeader.readLines("ace_cpp/engine/ObjectSystem/3D/ace.Layer3D.h")

engineHeader.readLines("ace_cpp/engine/ObjectSystem/ace.Scene.h")

engineHeader.readLines("ace_cpp/engine/ace.Engine.h")

engineHeader.output('include/ace.h')
