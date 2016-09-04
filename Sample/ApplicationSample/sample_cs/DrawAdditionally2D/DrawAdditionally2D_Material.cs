
/// <summary>
/// 階調を反転して画像を描画するオブジェクト
/// </summary>
class InvertedDrawnObject2D : asd.TextureObject2D
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
	asd.Texture2D texture;

	public InvertedDrawnObject2D()
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

		// 画像を読み込む。
		texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// 画像を設定する。
		material2d.SetTexture2D("g_texture", texture);
		material2d.SetTextureFilterType("g_texture", asd.TextureFilterType.Linear);
		material2d.SetTextureWrapType("g_texture", asd.TextureWrapType.Repeat);
	}

	protected override void OnDrawAdditionally()
	{
		// 画像を描画する。
		DrawSpriteWithMaterialAdditionally(
			new asd.Vector2DF(100, 100),
			new asd.Vector2DF(400, 100),
			new asd.Vector2DF(400, 400),
			new asd.Vector2DF(100, 400),
			new asd.Color(255, 255, 255, 255),
			new asd.Color(255, 255, 255, 255),
			new asd.Color(255, 255, 255, 255),
			new asd.Color(255, 255, 255, 255),
			new asd.Vector2DF(0.0f, 0.0f),
			new asd.Vector2DF(1.0f, 0.0f),
			new asd.Vector2DF(1.0f, 1.0f),
			new asd.Vector2DF(0.0f, 1.0f),
			material2d,
			asd.AlphaBlendMode.Blend,
			0);
	}
}

/// <summary>
/// 専用のシェーダーを使用してスプライトを追加で描画するサンプル。
/// </summary>
class DrawAdditionally2D_Material : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("DrawAdditionally2D_Material", 640, 480, new asd.EngineOption());

		// シーン、レイヤー、画像を表示するオブジェクトを生成する。
		var scene = new asd.Scene();
		var layer = new asd.Layer2D();
		var obj = new InvertedDrawnObject2D();

		// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		asd.Engine.ChangeScene(scene);
		scene.AddLayer(layer);
		layer.AddObject(obj);

		while (asd.Engine.DoEvents())
		{
			asd.Engine.Update();
			Recorder.TakeScreenShot("DrawAdditionally2D_Material", 5);
		}

		asd.Engine.Terminate();
	}

	public string Description
	{
		get { return "専用のシェーダーを使用してスプライトを追加で描画するサンプル。"; }
	}

	public string Title
	{
		get { return "追加描画(シェーダー)"; }
	}
}

