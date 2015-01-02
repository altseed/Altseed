static const char* deferred_buffer_ps_dx = R"(

Texture2D		g_gbuffer0Texture		: register( t0 );
SamplerState	g_gbuffer0Sampler		: register( s0 );

Texture2D		g_gbuffer1Texture		: register( t1 );
SamplerState	g_gbuffer1Sampler		: register( s1 );

Texture2D		g_gbuffer2Texture		: register( t2 );
SamplerState	g_gbuffer2Sampler		: register( s2 );

Texture2D		g_gbuffer3Texture		: register( t3 );
SamplerState	g_gbuffer3Sampler		: register( s3 );

Texture2D		g_shadowmapTexture		: register( t4 );
SamplerState	g_shadowmapSampler		: register( s4 );

Texture2D		g_ssaoTexture		: register( t5 );
SamplerState	g_ssaoSampler		: register( s5 );

Texture2D		g_environmentTexture		: register( t6 );
SamplerState	g_environmentSampler		: register( s6 );

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

float	flag;

float3 GetBaseColor(float2 uv)
{
	return g_gbuffer0Texture.Sample(g_gbuffer0Sampler, uv).xyz;
}

float4 GetSmoothnessMetalnessAO(float2 uv)
{
	return g_gbuffer1Texture.Sample(g_gbuffer1Sampler, uv).xyzw;
}

float3 GetNormal(float2 uv)
{
	return g_gbuffer2Texture.Sample(g_gbuffer2Sampler, uv).xyz;
}

float3 GetEnvironment(float2 uv)
{
	return g_environmentTexture.Sample(g_environmentSampler, uv).xyz;
}

float3 GetAO(float2 uv)
{
	float ao = g_ssaoTexture.Sample(g_ssaoSampler, uv).x;
	return float3(ao,ao,ao);
}

float3 CalcDiffuseColor(float3 baseColor, float metalness)
{
	return baseColor * (1.00000 - metalness);
}

float3 CalcSpecularColor(float3 baseColor, float metalness)
{
	float3 minColor = float3(0.0400000, 0.0400000, 0.0400000);
	return minColor.xyz * (1.0-metalness) + baseColor.xyz * metalness;
}

float4 main( const PS_Input Input ) : SV_Target
{
	float2 uv = Input.UV;
	// uv.y = 1.0 - uv.y;

	float4 color = float4(0.0,0.0,0.0,1.0);

	if(flag == 0.0)
	{
		float3 baseColor = GetBaseColor(uv);
		float metalness = GetSmoothnessMetalnessAO(uv).y;
		color.xyz = CalcDiffuseColor(baseColor,metalness);
	}
	else if(flag == 1.0)
	{
		color.xyz = GetNormal(uv);
		color.xyz = (color.xyz + float3(1.0,1.0,1.0)) * float3(0.5,0.5,0.5);
		color.xyz = pow(color.xyz, 2.2f);
	}
	else if(flag == 2.0)
	{
		float3 baseColor = GetBaseColor(uv);
		float metalness = GetSmoothnessMetalnessAO(uv).y;
		color.xyz = CalcSpecularColor(baseColor,metalness);
	}
	else if(flag == 3.0)
	{
		float s = GetSmoothnessMetalnessAO(uv).x;
		color.xyz = float3(s,s,s);
	}
	else if(flag == 4.0)
	{
		color.xyz = GetEnvironment(uv).xyz;
	}
	else if(flag == 5.0)
	{
		color.xyz = GetAO(uv).xyz;
	}

	return color;
}

)";
