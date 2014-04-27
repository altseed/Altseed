static const char* model_internal_ps_dx = R"(

//<|| モデル共通レジスタ
Texture2D		g_colorTexture		: register( t0 );
SamplerState	g_colorSampler		: register( s0 );
//>||
Texture2D		g_normalTexture		: register( t1 );
SamplerState	g_normalSampler		: register( s1 );

Texture2D		g_specularTexture		: register( t2 );
SamplerState	g_specularSampler		: register( s2 );

struct PS_Input
{
	float4 Position		: SV_POSITION;

	float4 Pos		: POSITION0;
	float4 Color	: Color0;
	half2 UV		: TEXCOORD0;
	half3 Normal	: NORMAL0;
};

struct PS_Output
{
	float4 Color		: SV_Target0;
	float4 NormalDepth	: SV_Target1;
	float4 Specular		: SV_Target2;
	float4 RoughnessMaterialAO		: SV_Target3;
	// ライトマップ系が入らない
}

float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = Input.Color;
	if(Output.a == 0.0f) discard;

	if(hasTextures.x != 0.0)
	{
		Output = Output * g_colorTexture.Sample(g_colorSampler, Input.UV);
	}
	if(Output.a == 0.0f) discard;

	// SSAO
	float2 ssaoUV = float2( (Input.Pos.x / Input.Pos.w + 1.0) / 2.0, (-Input.Pos.y / Input.Pos.w + 1.0) / 2.0 );
	float a = g_ssaoTexture.Sample(g_ssaoSampler, ssaoUV).x;
	//a = 1.0;

	// shadow
	float2 shadowUV = float2( (Input.LightPos.x / Input.LightPos.w + 1.0) / 2.0, 1.0 - (Input.LightPos.y / Input.LightPos.w + 1.0) / 2.0 );
	float lightDepthZ = Input.LightDepth;

	float2 shadowParam = g_shadowTexture.Sample(g_shadowSampler, shadowUV).xy;

	float shadow = VSM(shadowParam, lightDepthZ );


	Output.rgb = Output.rgb * shadow * a;
	return Output;
}

)";