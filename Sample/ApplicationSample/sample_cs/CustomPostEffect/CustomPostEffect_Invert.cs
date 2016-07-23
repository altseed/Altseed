
/// <summary>
/// 階調を反転させるポストエフェクト
/// </summary>
class CustomPostEffect_InvertPostEffect : asd.PostEffect
{
	/// <summary>
	/// DirectX向けシェーダー(HLSLで記述)
	/// </summary>
	private static string shader2d_dx_ps = @"
Texture2D g_texture : register( t0 );
SamplerState g_sampler : register( s0 );

struct PS_Input
{
	float4 SV_Position : SV_POSITION;
	float4 Position : POSITION;
	float2 UV : UV;
	float4 Color : COLOR;
};

float4 main( const PS_Input Input ) : SV_Target
{
	float4 color = g_texture.Sample(g_sampler, Input.UV);
	return float4( 1.0 - color.x, 1.0 - color.y, 1.0 - color.z, color.w);
}

";

	/// <summary>
	/// OpenGL向けシェーダー(GLSLで記述)
	/// </summary>
	private static string shader2d_gl_ps = @"
uniform sampler2D g_texture;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

void main()
{
	vec4 color = texture(g_texture, inUV.xy);
	outOutput = vec4( 1.0 - color.x, 1.0 - color.y, 1.0 - color.z, color.w);
}

";

	asd.Shader2D shader;
	asd.Material2D material2d;

	public CustomPostEffect_InvertPostEffect()
	{
		// シェーダーをHLSL/GLSLから生成する。
		if(asd.Engine.Graphics.GraphicsDeviceType == asd.GraphicsDeviceType.DirectX11)
		{
			shader = asd.Engine.Graphics.CreateShader2D(shader2d_dx_ps);
		}
		else if(asd.Engine.Graphics.GraphicsDeviceType == asd.GraphicsDeviceType.OpenGL)
		{
			shader = asd.Engine.Graphics.CreateShader2D(shader2d_gl_ps);
		}

		// シェーダーからマテリアルを生成する。
		material2d = asd.Engine.Graphics.CreateMaterial2D(shader);
	}

	protected override void OnDraw(asd.RenderTexture2D dst, asd.RenderTexture2D src)
	{
		// マテリアルを経由してシェーダー内のg_texture変数に画面の画像(src)を入力する。
		material2d.SetTexture2D("g_texture", src);

		// 出力画像(dst)に指定したマテリアルで描画する。
		DrawOnTexture2DWithMaterial(dst, material2d);
	}
}

/// <summary>
/// 階調を反転するポストエフェクトのサンプル。
/// </summary>
class CustomPostEffect_Invert : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("CustomPostEffect_Invert", 640, 480, new asd.EngineOption());

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
		layer.AddPostEffect(new CustomPostEffect_InvertPostEffect());

		while (asd.Engine.DoEvents())
		{
			asd.Engine.Update();
			Recorder.TakeScreenShot("CustomPostEffect_Invert", 5);
		}

		asd.Engine.Terminate();
	}

	public string Description
	{
		get { return "画面の色の階調を反転するポストエフェクトのサンプル。"; }
	}

	public string Title
	{
		get { return "反転ポストエフェクト"; }
	}
}

