using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
	class TextureObject2DWithMaterial : EngineTest
	{

		static string shader2d_dx_ps = @"

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float3			g_values		: register( c0 );

struct PS_Input
{
	float4 SV_Position	: SV_POSITION;
	float4 Position		: POSITION;
	float2 UV			: UV;
	float4 Color		: COLOR;
};

float4 main( const PS_Input Input ) : SV_Target
{
	float4 outputedColor = g_texture.Sample(g_sampler, Input.UV);
	if(outputedColor.a == 0.0f) discard;

	float2 localPos = float2( (Input.Position.x + 1.0) / 2.0 * g_values.x, (Input.Position.y + 1.0) / 2.0 * g_values.y );
	float2 centerPos = float2( g_values.x / 2.0, g_values.y / 2.0 );

	if( (localPos.x - centerPos.x) * (localPos.x - centerPos.x) + (localPos.y - centerPos.y) * (localPos.y - centerPos.y) < g_values.z * g_values.z )
	{
		outputedColor.r = 1.0 - outputedColor.r;
		outputedColor.g = 1.0 - outputedColor.g;
		outputedColor.b = 1.0 - outputedColor.b;
	}

	return outputedColor;
}

";

		static string shader2d_gl_ps = @"

uniform sampler2D	g_texture;

uniform vec3		g_values;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

void main()
{
	vec4 outputedColor = texture(g_texture, inUV.xy);

	vec2 localPos = vec2( (inPosition.x + 1.0) / 2.0 * g_values.x, (inPosition.y + 1.0) / 2.0 * g_values.y );
	vec2 centerPos = vec2( g_values.x / 2.0, g_values.y / 2.0 );

	if( (localPos.x - centerPos.x) * (localPos.x - centerPos.x) + (localPos.y - centerPos.y) * (localPos.y - centerPos.y) < g_values.z * g_values.z )
	{
		outputedColor.r = 1.0 - outputedColor.r;
		outputedColor.g = 1.0 - outputedColor.g;
		outputedColor.b = 1.0 - outputedColor.b;
	}

	outOutput = outputedColor;
}

";

		public TextureObject2DWithMaterial()
			: base(20)
		{
		}

		protected override void OnStart()
		{
			asd.Shader2D shader;
			asd.Material2D material2d;

			var g = asd.Engine.Graphics;

			if (g.GraphicsDeviceType == asd.GraphicsDeviceType.DirectX11)
			{
				shader = g.CreateShader2D(
					shader2d_dx_ps
					);
			}
			else if (g.GraphicsDeviceType == asd.GraphicsDeviceType.OpenGL)
			{
				shader = g.CreateShader2D(
					shader2d_gl_ps
					);
			}
			else
			{
				throw new Exception();
			}

			material2d = asd.Engine.Graphics.CreateMaterial2D(shader);
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
			var textureObj1 = new asd.TextureObject2D();
			var textureObj2 = new asd.TextureObject2D();

			layer.AddObject(textureObj1);
			layer.AddObject(textureObj2);
			scene.AddLayer(layer);
			asd.Engine.ChangeScene(scene);

			textureObj1.Texture = texture;
			textureObj1.Src = new asd.RectF(256, 256, 256, 256);
			textureObj1.Position = new asd.Vector2DF(320, 240);
			textureObj1.Material = material2d;

			textureObj2.Texture = texture;
			textureObj2.Src = new asd.RectF(0, 0, 256, 256);
			textureObj2.Position = new asd.Vector2DF(0, 0);
			textureObj2.TurnLR = true;
			textureObj2.Material = material2d;
		}

		protected override void OnUpdating()
		{

		}
	}
}