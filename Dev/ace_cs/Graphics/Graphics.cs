using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Graphics
	{
		swig.Graphics_Imp graphics;

		internal Graphics(swig.Graphics_Imp swig)
		{
			graphics = swig;
		}

		public GraphicsType GraphicsType
		{
			get { return (GraphicsType)graphics.GetGraphicsType(); }
		}

		public Texture2D CreateTexture2D(string path)
		{
			var texture = graphics.CreateTexture2D_Imp(path);
			var p = texture.GetPtr();

			var existing = GC.Texture2Ds.GetObject(p);
			if (existing != null)
			{
				return existing;
			}

			var ret = new Texture2D(texture);
			GC.Texture2Ds.AddObject(p, ret);
			return ret;
		}

		public RenderTexture2D CreateRenderTexture2D(int width, int height, TextureFormat format)
		{
			var rt = graphics.CreateRenderTexture_Imp(width,height, (swig.eTextureFormat)format);
			var p = rt.GetPtr();

			var existing = GC.Texture2Ds.GetObject(p);
			if (existing != null)
			{
				return (RenderTexture2D)existing;
			}

			var ret = new RenderTexture2D(rt);
			GC.Texture2Ds.AddObject(p, ret);
			return ret;
		}

		public Shader2D CreateShader2D(string shaderText, ShaderVariableProperty[] variableProperties)
		{
			swig.ShaderVariablePropertyVector vprops = new swig.ShaderVariablePropertyVector();
			foreach (var v_ in variableProperties)
			{
				var sprop = new swig.ShaderVariableProperty();
				sprop.Name = v_.Name;
				sprop.Type = (swig.ShaderVariableType)v_.Type;
				sprop.Offset = v_.Offset;
				vprops.Add(sprop);
			}

			var shader = graphics.CreateShader2D_Imp(shaderText, vprops);
			if (shader == null) return null;

			var p = shader.GetPtr();

			var existing = GC.Shader2Ds.GetObject(p);
			if (existing != null)
			{
				return existing;
			}

			var ret = new Shader2D(shader);
			GC.Shader2Ds.AddObject(p, ret);
			return ret;
		}

		public Material2D CreateMaterial2D(Shader2D shader)
		{
			var material = graphics.CreateMaterial2D_Imp(shader.SwigObject);
			var p = material.GetPtr();

			var existing = GC.Material2Ds.GetObject(p);
			if (existing != null)
			{
				return existing;
			}

			var ret = new Material2D(material);
			GC.Material2Ds.AddObject(p, ret);
			return ret;
		}
	}
}
