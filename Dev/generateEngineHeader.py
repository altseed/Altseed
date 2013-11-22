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

			if re.search('#include <ace.', line) != None:
				line = f.readline()
				continue;

			self.lines.append(line)
			line = f.readline()
		self.lines.append('\n')
		f.close

	def output(self,path):
		f =  codecs.open(path, 'w','utf-8_sig')
		for line in self.lines:
			f.write(line)
		f.close()

engineHeader = CreateHeader()
engineHeader.append('#pragma once')
engineHeader.readLines("ace_cpp/common/ace.common.Base.h")
engineHeader.readLines("ace_cpp/engine/ace.Engine.Base.h")

engineHeader.readLines("ace_cpp/common/Utility/ace.BinaryReader.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.BinaryWriter.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.GetTypeName.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.PointerArray.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.Timer.h")
engineHeader.readLines("ace_cpp/common/Utility/ace.TypeErasureCopy.h")

engineHeader.readLines("ace_cpp/common/Math/ace.Vector2DF.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Vector2DI.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Vector3DF.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Matrix33.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Matrix43.h")
engineHeader.readLines("ace_cpp/common/Math/ace.Matrix44.h")
engineHeader.readLines("ace_cpp/common/Math/ace.RectF.h")

engineHeader.readLines("ace_cpp/common/Graphics/ace.Color.h")

engineHeader.readLines("ace_cpp/core/ace.Core.Base.h")
engineHeader.readLines("ace_cpp/core/ace.ReferenceObject.h")

engineHeader.readLines("ace_cpp/core/Input/ace.IMouseButtonState.h")
engineHeader.readLines("ace_cpp/core/Input/ace.IMouseWheelableButtonState.h");
engineHeader.readLines("ace_cpp/core/Input/ace.SideButton.h")
engineHeader.readLines("ace_cpp/core/Input/ace.MiddleButton.h")
engineHeader.readLines("ace_cpp/core/Input/ace.Mouse.h")
engineHeader.readLines("ace_cpp/core/Input/ace.Keyboard.h")

engineHeader.readLines("ace_cpp/core/Input/ace.Joystick.h")
engineHeader.readLines("ace_cpp/core/Input/ace.JoystickContainer.h")

engineHeader.readLines("ace_cpp/core/Log/ace.Log.h")
engineHeader.readLines("ace_cpp/core/Profiler/ace.Profiler.h")

engineHeader.readLines("ace_cpp/core/Graphics/Common/Resource/ace.Texture2D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Common/Resource/ace.RenderTexture.h")
engineHeader.readLines("ace_cpp/core/Graphics/Common/Resource/ace.Shader2D.h")
engineHeader.readLines("ace_cpp/core/Graphics/Common/Resource/ace.Material2D.h")

engineHeader.readLines("ace_cpp/core/Graphics/Common/3D/ace.Mesh.h")

engineHeader.readLines("ace_cpp/core/Graphics/Common/ace.Graphics.h")

engineHeader.readLines("ace_cpp/core/Core/ace.Core.h")

engineHeader.readLines("ace_cpp/core/ObjectSystem/ace.eChildMode.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffect.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectGrayScale.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectSepia.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/PostEffect/ace.PostEffectGaussianBlur.h")
engineHeader.readLines('ace_cpp/engine/ObjectSystem/Component/ace.Object2DComponent.h')
engineHeader.readLines('ace_cpp/engine/ObjectSystem/Component/ace.Layer2DComponent.h')
engineHeader.readLines('ace_cpp/engine/ObjectSystem/Component/ace.SceneComponent.h')

engineHeader.readLines("ace_cpp/engine/ObjectSystem/ace.Layer.h")

engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.Object2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.TextureObject2D.h")
engineHeader.readLines("ace_cpp/engine/ObjectSystem/2D/ace.Layer2D.h")

engineHeader.readLines("ace_cpp/engine/ObjectSystem/ace.Scene.h")

engineHeader.readLines("ace_cpp/engine/ace.Engine.h")

engineHeader.output('include/ace.h')
