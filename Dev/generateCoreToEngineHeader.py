import re
import codecs

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

fromCoreToEngine = CreateHeader()
fromCoreToEngine.append('#pragma once')
fromCoreToEngine.append('\n#include <stdint.h>')
fromCoreToEngine.append('\n#include <ace.common.Base.h>')


fromCoreToEngine.readLines("ace_cpp/common/Math/ace.Vector2DI.h")
fromCoreToEngine.readLines("ace_cpp/common/Math/ace.Vector2DF.h")
fromCoreToEngine.readLines("ace_cpp/common/Math/ace.RectI.h")
fromCoreToEngine.readLines("ace_cpp/common/Math/ace.RectF.h")
fromCoreToEngine.append('namespace ace{ struct Vector3DF; }')
fromCoreToEngine.readLines("ace_cpp/common/Math/ace.Matrix44.h")
fromCoreToEngine.readLines("ace_cpp/common/Math/ace.Vector3DF.h")
fromCoreToEngine.readLines("ace_cpp/common/Math/ace.Vector4DF.h")
fromCoreToEngine.readLines("ace_cpp/common/Math/ace.Matrix33.h")

fromCoreToEngine.readLines("ace_cpp/common/Graphics/ace.Graphics.Common.h")
fromCoreToEngine.readLines("ace_cpp/common/Graphics/ace.Color.h")

fromCoreToEngine.readLines("ace_cpp/core/ace.Core.Base.h")
fromCoreToEngine.readLines("ace_cpp/core/ace.ReferenceObject.h")

fromCoreToEngine.readLines("ace_cpp/core/Input/ace.IMouseButtonState.h")
fromCoreToEngine.readLines("ace_cpp/core/Input/ace.IMouseWheelableButtonState.h");
fromCoreToEngine.readLines("ace_cpp/core/Input/ace.SideButton.h")
fromCoreToEngine.readLines("ace_cpp/core/Input/ace.MiddleButton.h")
fromCoreToEngine.readLines("ace_cpp/core/Input/ace.Mouse.h")
fromCoreToEngine.readLines("ace_cpp/core/Input/ace.Keyboard.h")
fromCoreToEngine.readLines("ace_cpp/core/Input/ace.Joystick.h")
fromCoreToEngine.readLines("ace_cpp/core/Input/ace.JoystickContainer.h")
fromCoreToEngine.readLines("ace_cpp/core/Log/ace.Log.h")
fromCoreToEngine.readLines("ace_cpp/core/Profiler/ace.Profile.h")
fromCoreToEngine.readLines("ace_cpp/core/Profiler/ace.Profiler.h")

fromCoreToEngine.readLines("ace_cpp/core/Sound/ace.SoundSource.h")
fromCoreToEngine.readLines("ace_cpp/core/Sound/ace.Sound.h")

fromCoreToEngine.readLines("ace_cpp/core/Graphics/Resource/ace.Texture.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Resource/ace.Texture2D.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Resource/ace.RenderTexture2D.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Resource/ace.CubemapTexture.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Resource/ace.Shader2D.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Resource/ace.Material2D.h")

fromCoreToEngine.readLines("ace_cpp/core/Graphics/Resource/ace.Effect.h")

fromCoreToEngine.readLines("ace_cpp/core/Graphics/3D/ace.Deformer.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/3D/ace.Mesh.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/3D/ace.Model.h")

fromCoreToEngine.readLines("ace_cpp/core/Graphics/Animation/ace.KeyframeAnimation.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Animation/ace.AnimationSource.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Animation/ace.AnimationClip.h")
fromCoreToEngine.readLines("ace_cpp/core/Graphics/Animation/ace.AnimationSystem.h")

fromCoreToEngine.readLines("ace_cpp/core/Graphics/ace.Graphics.h")

fromCoreToEngine.readLines("ace_cpp/core/Core/ace.Core.h")
fromCoreToEngine.readLines("ace_cpp/core/Core/ace.CreateCore.h")

fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/ace.eChildMode.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/ace.CoreLayer.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/ace.CoreScene.h')

fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/2D/ace.CoreObject2D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/2D/ace.CoreTextureObject2D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/2D/ace.CoreCameraObject2D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/2D/ace.CoreTextObject2D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/2D/ace.CoreEffectObject2D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/2D/ace.CoreLayer2D.h')

fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/3D/ace.CoreObject3D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/3D/ace.CoreModelObject3D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/3D/ace.CoreDirectionalLightObject3D.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/3D/ace.CoreCameraObject3D.h')
fromCoreToEngine.readLines("ace_cpp/core/ObjectSystem/3D/ace.CoreEffectObject3D.h")
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/3D/ace.CoreLayer3D.h')

fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/PostEffect/ace.CorePostEffect.h')
fromCoreToEngine.readLines('ace_cpp/core/ObjectSystem/ace.ObjectSystemFactory.h')

fromCoreToEngine.output('ace_cpp/engine/ace.CoreToEngine.h')
