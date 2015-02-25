## GPU

### デバッガー

*DirectX11の場合
VisualStudio付属のツール(Professional以上)
デバッグ->グラフィックス->診断の開始

*Nvidiaのビデオカードの場合
(Nsight)[http://www.nvidia.co.jp/object/parallel-nsight-jp.html]

*AMDの場合
CodeXL[http://developer.amd.com/tools-and-sdks/heterogeneous-computing/codexl/]

### OpenGL(Intel)の不具合

* 同じ関数内でtextureで取得した値を2回returnすると落ちる。

Intel HD Graphics4000,4400で確認
ドライバのバージョン不明

FRAGが定義されていると落ちる。

```
vec4 SampleTexture(sampler2D texture_, int sampler_, vec2 uv_){
	return texture(texture_, uv_);
}

vec4 GetColor(vec2 uv)
{
#ifdef FLAG
	return SampleTexture( g_texture, g_sSampler, uv );
#endif
	return SampleTexture( g_texture, g_sSampler, uv );
}
```

以下のように修正すると落ちない。

```
vec4 SampleTexture(sampler2D texture_, int sampler_, vec2 uv_){
	return texture(texture_, uv_);
}

vec4 GetColor(vec2 uv)
{
#ifdef FLAG
	return SampleTexture( g_texture, g_sSampler, uv );
#else
	return SampleTexture( g_texture, g_sSampler, uv );
#endif
}
```