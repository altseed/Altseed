
/// <summary>
/// 一部の領域にモザイクをかけるポストエフェクトのサンプル。
/// </summary>
class CustomPostEffect_Mosaic : ISample
{
	/// <summary>
	/// DirectX向けシェーダー(HLSLで記述)
	/// </summary>
	static string shader2d_dx_ps = @"
Texture2D g_texture;
SamplerState g_sampler;

float2 g_windowSize;
float4 g_area;

struct PS_Input
{
	float4 SV_Position	: SV_POSITION;
	float4 Position		: POSITION;
	float2 UV			: UV;
	float4 Color		: COLOR;
};

float2 GetPixelPosition(float2 uv)
{
	return g_windowSize * uv;
}

float4 main( const PS_Input Input ) : SV_Target
{
	float2 pixelPos = GetPixelPosition(Input.UV);

	if(
		g_area.x < pixelPos.x &&
		g_area.x + g_area.z > pixelPos.x &&
		g_area.y < pixelPos.y &&
		g_area.y + g_area.w > pixelPos.y)
	{
		float2 uv = Input.UV;
		uv.x = floor(uv.x * g_windowSize.x / 5.0) * 5.0 / g_windowSize.x;
		uv.y = floor(uv.y * g_windowSize.y / 5.0) * 5.0 / g_windowSize.y;
		return g_texture.Sample(g_sampler, uv);
	}
	else
	{
		return g_texture.Sample(g_sampler, Input.UV);
	}
}

";
	/// <summary>
	/// OpenGL向けシェーダー(GLSLで記述)
	/// </summary>
	static string shader2d_gl_ps = @"
uniform sampler2D g_texture;

uniform vec2 g_windowSize;
uniform vec4 g_area;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

vec2 GetPixelPosition(vec2 uv)
{
	uv.y = 1.0 - uv.y;
	return g_windowSize * uv;
}

void main()
{
	vec2 pixelPos = GetPixelPosition(inUV);

	if(
		g_area.x < pixelPos.x &&
		g_area.x + g_area.z > pixelPos.x &&
		g_area.y < pixelPos.y &&
		g_area.y + g_area.w > pixelPos.y)
	{
		vec2 uv = inUV;
		uv.x = floor(uv.x * g_windowSize.x / 5.0) * 5.0 / g_windowSize.x;
		uv.y = floor(uv.y * g_windowSize.y / 5.0) * 5.0 / g_windowSize.y;
		outOutput = texture(g_texture, uv);
	}
	else
	{
		outOutput = texture(g_texture, inUV);
	}
}

";

	/// <summary>
	/// 一部の領域にモザイクをかけるポストエフェクト
	/// </summary>
	class MosaicPostEffect : asd.PostEffect
	{
		asd.Shader2D shader;
		asd.Material2D material2d;

		public MosaicPostEffect()
		{
			// シェーダーをHLSL/GLSLから生成する。
			if (asd.Engine.Graphics.GraphicsDeviceType == asd.GraphicsDeviceType.DirectX11)
			{
				shader = asd.Engine.Graphics.CreateShader2D(
					shader2d_dx_ps
					);
			}
			else if (asd.Engine.Graphics.GraphicsDeviceType == asd.GraphicsDeviceType.OpenGL)
			{
				shader = asd.Engine.Graphics.CreateShader2D(
					shader2d_gl_ps
					);
			}

			// シェーダーからマテリアルを生成する。
			material2d = asd.Engine.Graphics.CreateMaterial2D(shader);
		}

		public override void OnDraw(asd.RenderTexture2D dst, asd.RenderTexture2D src)
		{
			// マテリアルを経由してシェーダー内のg_texture変数に画面の画像(src)を入力する。
			material2d.SetTexture2D("g_texture", src);

			// マテリアルを経由してシェーダー内のg_area変数にポストエフェクトを適用する範囲を入力する。
			material2d.SetVector2DF("g_windowSize", new asd.Vector2DF(asd.Engine.WindowSize.X, asd.Engine.WindowSize.Y));

			// マテリアルを経由してシェーダー内のg_area変数にポストエフェクトを適用する範囲を入力する。
			material2d.SetVector4DF("g_area", new asd.Vector4DF(50,50, 200, 200));

			// 出力画像(dst)に指定したマテリアルで描画する。
			DrawOnTexture2DWithMaterial(dst, material2d);
		}
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("CustomPostEffect_Mosaic", 640, 480, new asd.EngineOption());

		// シーン、レイヤー、画像を表示するオブジェクトを生成する。
		var scene = new asd.Scene();
		var layer = new asd.Layer2D();
		var obj = new asd.TextureObject2D();
		obj.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		asd.Engine.ChangeScene(scene);
		scene.AddLayer(layer);
		layer.AddObject(obj);

		// レイヤーにポストエフェクトを適用する。
		layer.AddPostEffect(new MosaicPostEffect());

		while (asd.Engine.DoEvents())
		{
			asd.Engine.Update();
			Recorder.TakeScreenShot("CustomPostEffect_Mosaic", 5);
		}

		asd.Engine.Terminate();
	}
}

