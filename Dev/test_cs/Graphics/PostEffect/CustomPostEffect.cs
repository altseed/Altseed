using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace test_cs.Graphics.PostEffect
{
	class CustomPostEffect : ISample
	{

		static string shader2d_dx_ps = @"

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 output = g_texture.Sample(g_sampler, Input.UV);
	if(output.a == 0.0f) discard;
	output.r = 1.0 - output.r;
	output.g = 1.0 - output.g;
	output.b = 1.0 - output.b;
	return output;
}

";

		static string shader2d_gl_ps = @"

uniform sampler2D g_texture;

void main()
{
	float4 output = texture2D(g_texture, inUV.xy);
	output.r = 1.0 - output.r;
	output.g = 1.0 - output.g;
	output.b = 1.0 - output.b;
	gl_FragColor = output; 
}

";

		class PostEffect : ace.PostEffect
		{

			Shader2D m_shader;
			Material2D m_material2d;

			public PostEffect(ace.Graphics g)
			{
				List<ace.ShaderVariableProperty> props = new List<ShaderVariableProperty>();
				var prop_tex = new ace.ShaderVariableProperty();
				prop_tex.Name = "g_texture";
				prop_tex.Offset = 0;
				prop_tex.Type = ace.ShaderVariableType.Texture2D;
				props.Add(prop_tex);

				if (g.GraphicsType == GraphicsType.DirectX11)
				{
					m_shader = g.CreateShader2D(
						shader2d_dx_ps,
						props.ToArray()
						);
				}
				else if (g.GraphicsType == GraphicsType.OpenGL)
				{
					m_shader = g.CreateShader2D(
						shader2d_gl_ps,
						props.ToArray()
						);
				}
				else
				{
					throw new Exception();
				}

				m_material2d = g.CreateMaterial2D(m_shader);
			}

			public override void OnDraw(ace.RenderTexture2D dst, ace.RenderTexture2D src)
			{
				m_material2d.SetTexture2D("g_texture", src);

				DrawOnTexture2DWithMaterial(dst, m_material2d);
			}
		}

		public void Run()
		{
			Engine.Initialize("CustomPostEffect", 640, 480, false);

			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D()
			{
				Texture = Engine.Graphics.CreateTexture2D("Data/Texture/Cloud1.png"),
				Scale = new Vector2DF(2.0f, 2.0f)
			};

			layer.AddObject(obj);
			layer.AddPostEffect(new PostEffect(Engine.Graphics));
			scene.AddLayer(layer);
			Engine.ChangeScene(scene);

			while (Engine.DoEvents())
			{
				Engine.Update();
			}

			Engine.Terminate();
		}

	}
}
