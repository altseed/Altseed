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
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 Color			: Color0;
	half2 UV				: TEXCOORD0;
	half3 Normal			: NORMAL0;
	half3 Binormal			: BINORMAL0;
	half3 Tangent			: TANGENT0;
};

#ifdef EXPORT_DEPTH
struct PS_Output
{
	float4 Depth					: SV_Target0;
};
#else
struct PS_Output
{
	float4 DiffuseColor					: SV_Target0;
	float4 SpecularColor_Smoothness		: SV_Target1;
	float4 NormalDepth					: SV_Target2;
	float4 AO_MatID						: SV_Target3;
};
#endif


half3 CalculateNormal( half3 normal, half3 tangent, half3 binormal, float3 normalMap )
{
	half3 n = (normalMap - 0.5) * 2.0;
	return mul( half3x3(tangent, binormal, normal), n );
}

half CalculateDepth(float4 position)
{
	return position.z / position.w;
}

PS_Output main( const PS_Input Input )
{
	float4 diffuseColor = Input.Color;
	if(diffuseColor.a == 0.0f) discard;

	diffuseColor = diffuseColor * g_colorTexture.Sample(g_colorSampler, Input.UV);
	if(diffuseColor.a == 0.0f) discard;	

	PS_Output Output;

#ifdef EXPORT_DEPTH
	Output.Depth.x = CalculateDepth( Input.Position );
	Output.Depth = float4( Output.Depth.x, Output.Depth.y, Output.Depth.z, Output.Depth.w );

#else

	Output.DiffuseColor = diffuseColor;

	Output.NormalDepth.xyz = CalculateNormal( Input.Normal, Input.Tangent, Input.Binormal, g_normalTexture.Sample(g_normalSampler, Input.UV).xyz );
	Output.NormalDepth.w = CalculateDepth( Input.Position );

	Output.SpecularColor_Smoothness.xyz = g_specularTexture.Sample(g_specularSampler, Input.UV).xyz;
	Output.SpecularColor_Smoothness.w = 0.5;

	Output.AO_MatID.x = 1.0;
	Output.AO_MatID.y = 0;
	Output.AO_MatID.z = 0;
	Output.AO_MatID.w = 0;

#endif

	return Output;
}

)";