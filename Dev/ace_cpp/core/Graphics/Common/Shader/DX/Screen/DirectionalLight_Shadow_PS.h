static const char* directonalLight_Shadow_ps_dx = R"(

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

float4x4		g_screenToShadowmap		: register( c0 );


struct PS_Input
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

float GetDepth(float2 uv)
{
	float w = g_gbuffer2Sampler.Sample(g_gbuffer2Sampler, uv);
	return w;
}

float2 GetShadowmapUV(float4 position, float2 uv)
{
	float w = g_gbuffer2Sampler.Sample(g_gbuffer2Sampler, uv);
	float4 originalProjectedPos = float4(position.x, position.y, w, 1.0);
	float4 shadowmapProjectedPos = mul(g_screenToShadowmap, originalProjectedPos);
	return float2( (shadowmapProjectedPos.x + 1.0) / 2.0f, -(shadowmapProjectedPos.y + 1.0) / 2.0f );
}

float VSM(float2 moments, float t)
{
	float ex = moments.x;
	float ex2 = moments.y;

	float p = 0.0;
	if (t <= ex) p = 1.0;

	float variance = ex2 - ex * ex;
	variance = max(variance, 0.4 / (depthParams_.x * depthParams_.x));

	float d = t - ex;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}

float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = float4(0.0,0.0,0.0,1.0);

	float2 shadowUV = GetShadowmapUV(Input.Position, Input.UV);
	float depth =  GetDepth( Input.UV);

	float2 shadowParam = g_shadowmapTexture.Sample(g_shadowmapSampler, shadowUV).xy;

	float shadow = VSM(shadowParam, lightDepthZ );

	Output.x = shadow;

	return Output;
}

)";
