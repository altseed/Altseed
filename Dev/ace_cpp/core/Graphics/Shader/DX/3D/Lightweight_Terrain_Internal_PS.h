static const char* lightweight_terrain_internal_ps_dx = R"(

//<|| モデル共通レジスタ
Texture2D		g_colorTexture		: register( t0 );
SamplerState	g_colorSampler		: register( s0 );
//>||

Texture2D		g_densityTexture		: register( t4 );
SamplerState	g_densitySampler		: register( s4 );

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 Color			: Color0;
	half2 UV				: TEXCOORD0;
	half2 UVSub				: UVSub0;
	half3 Normal			: NORMAL0;
};

float4 main( const PS_Input Input ) : SV_Target
{
#ifdef BLACK
	return float4(0.0,0.0,0.0,1.0);
#endif

	float4 Output = Input.Color;
	if(Output.a == 0.0f) discard;

	Output = Output * g_colorTexture.Sample(g_colorSampler, Input.UV);
	if(Output.a == 0.0f) discard;

	float density = g_densityTexture.Sample(g_densitySampler, Input.UVSub).x;
	Output = Output * density;

	return Output;
}

)";
