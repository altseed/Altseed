using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public enum ShaderVariableType
	{
		Float = swig.ShaderVariableType.SHADER_VARIABLE_TYPE_FLOAT,
		Vector2DF = swig.ShaderVariableType.SHADER_VARIABLE_TYPE_VECTOR2DF,
		Vector3DF = swig.ShaderVariableType.SHADER_VARIABLE_TYPE_VECTOR3DF,
		Texture2D = swig.ShaderVariableType.SHADER_VARIABLE_TYPE_TEXTURE2D,
	}

	public class ShaderVariableProperty
	{
		public string Name = string.Empty;

		public ShaderVariableType Type;

		public int Offset = 0;
	}
}
