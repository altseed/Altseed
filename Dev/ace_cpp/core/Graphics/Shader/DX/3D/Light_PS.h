static const char* light_ps_dx = R"(

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

float4x4		g_cameraPositionToShadowCameraPosition;
float4x4		g_shadowProjection;

float4 reconstructInfo1;
float4 reconstructInfo2;

float3		directionalLightDirection;
float3		directionalLightColor;
float3		skyLightColor;
float3		groundLightColor;
float3		upDir;

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

//<|| ALSL
float3 calcAmbientColor(float3 upDir, float3 normal)
{
	float3 color = float3(0.000000, 0.000000, 0.000000);
	float NoU = dot(normal, upDir);
	color.xyz = color.xyz + skyLightColor * max(NoU + 1, 0.000000) / 2.00000;
	color.xyz = color.xyz + groundLightColor * max(-NoU + 1, 0.000000) / 2.00000;
	return color;
}

float3 calcDirectionalLightColor(float3 normal, float3 lightDir, float shadow)
{
	float3 color = float3(0.000000, 0.000000, 0.000000);
	float NoL = dot(normal, lightDir);
	color.xyz = directionalLightColor * max(NoL, 0.000000) * shadow;
	return color;
}

float VSM(float2 moments, float t)
{
	float ex = moments.x;
	float ex2 = moments.y;
	float p = 0.000000;
	if(t <= ex) p = 1.00000;
	float variance = ex2 - ex * ex;
	variance = max(variance, 0.400000 / (reconstructInfo1.x * reconstructInfo1.x));
	float d = t - ex;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}


//||>

float3 GetDiffuseColor(float2 uv)
{
	return g_gbuffer0Texture.Sample(g_gbuffer0Sampler, uv).xyz;
}

float3 GetNormal(float2 uv)
{
	return g_gbuffer2Texture.Sample(g_gbuffer2Sampler, uv).xyz;
}

float GetNormalizedDepth(float2 uv)
{
	return g_gbuffer2Texture.Sample(g_gbuffer2Sampler, uv).w;
}

float ReconstructDepth(float z)
{
	return -((z * reconstructInfo1.x) + reconstructInfo1.y);
}

float3 ReconstructPosition(float2 screenXY, float depth)
{
	return float3( reconstructInfo2.xy * screenXY * (-depth), depth );
}

float4 ReconstructShadowmapPosition(float3 cameraPos)
{
	float4 shadowmapPos = mul( g_cameraPositionToShadowCameraPosition, float4(cameraPos,1.0) );
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

float4 ReconstructProjectedShadowmapPosition(float4 shadowmapPosition)
{
	float4 shadowmapPos = mul( g_shadowProjection, shadowmapPosition );
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

float2 GetShadowmapUV(float4 shadowmapPos)
{
	return float2( (shadowmapPos.x + 1.0) / 2.0f, 1.0 - (shadowmapPos.y + 1.0) / 2.0f );
}

float4 main( const PS_Input Input ) : SV_Target
{
	float2 uv = Input.UV;
	// uv.y = 1.0 - uv.y;

	float3 cameraPos = ReconstructPosition(Input.Position.xy, ReconstructDepth(GetNormalizedDepth(uv)));

	float4 lightColor = float4(0.0,0.0,0.0,1.0);
	float3 normal = GetNormal(uv);

#ifdef DIRECTIONAL_LIGHT
	float4 shadowmapPos = ReconstructShadowmapPosition(cameraPos);
	float4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos);
	float2 shadowmapUV = GetShadowmapUV(projShadowmapPos);

	float depth = (-shadowmapPos.z - reconstructInfo1.y) / reconstructInfo1.x;
	float2 shadowParam = g_shadowmapTexture.Sample(g_shadowmapSampler, shadowmapUV).xy;

	float shadow = VSM(shadowParam, depth );

	lightColor.xyz += calcDirectionalLightColor(normal, directionalLightDirection, shadow);
#endif

#ifdef AMBIENT_LIGHT
	lightColor.xyz += calcAmbientColor(upDir, normal);
#endif

	float ao = g_ssaoTexture.Sample(g_ssaoSampler, uv).x;
	lightColor.xyz *= ao;

	lightColor.xyz *= GetDiffuseColor(uv);

	return lightColor;
}

)";
