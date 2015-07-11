## シェーダー2D (Shader2D)

### 概要

[ポストエフェクト](./../PostEffect/PostEffect.md)をユーザーが実装するために使用するシェーダークラスです。
AltseedはHLSL(DirectX)やGLSL(OpenGL)によりシェーダーを記述して画像に対する効果を記述できます。

HLSLのバージョンは4、GLSLのバージョンは3.3になります。

このクラスのインスタンスはグラフィッククラスのCreateShader2Dにシェーダーのソースコードを渡すことによって生成できます。
このクラス単体では使用せず、マテリアル2D(Material2D)と組み合わせて使用します。
マテリアル2Dにシェーダーを適用しすることで、マテリアル2Dを経由して画像を加工することができます。

シェーダー2Dは自由に書けるわけではなく、固定部分が存在します。

頂点シェーダーは記述できず、ピクセルシェーダー（フラグメントシェーダー）のみを記述します。

* HLSLの場合

```
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
	// 現在の画素の値を取得し、何も変更しないまま返す関数になっています。
	float4 color = g_texture.Sample(g_sampler, Input.UV);
	return color;
}
```

* GLSLの場合

```
uniform sampler2D	g_texture;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

void main()
{
	// 現在の画素の値を取得し、何も変更しないまま返す関数になっています。
	vec4 color = texture(g_texture, inUV.xy);
	outOutput = color;
}
```

固定部分は頂点レイアウト、main関数、出力先になります。

頂点レイアウトは、座標、UV、頂点カラー、となります。

座標は、左上が(-1,1)、右下が(1,-1)となります。

UVはHLSLの場合、左上が(0,0)、GLSLの場合、左上が(0,1)になります。

関数だけでなく、変数も渡すことができます。変数はマテリアル2D経由で指定します。
HLSLの場合、レジスタを記述する必要はありません。

### 主なメソッド

なし

### 主なプロパティ

なし

### 主なイベント

なし

### 使用方法

階調を反転させるサンプルです。

* include_application_sample CustomPostEffect_Invert