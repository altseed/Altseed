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

Texture2D		g_environmentDiffuseTexture;
SamplerState	g_environmentDiffuseSampler;

float4x4		g_cameraPositionToShadowCameraPosition;
float4x4		g_shadowProjection;

float3 reconstructInfo1;
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
float calcLightingGGX(float3 N, float3 V, float3 L, float roughness, float F0)
{
	float alpha = roughness * roughness;
	float3 H = normalize(V + L);
#ifdef OPENGL
	float dotNL = clamp(dot(N, L), 0.000000, 1.00000);
	float dotLH = clamp(dot(L, H), 0.000000, 1.00000);
	float dotNH = clamp(dot(N, H), 0.000000, 1.00000);
#endif
#ifdef DIRECTX
	float dotNL = saturate(dot(N, L));
	float dotLH = saturate(dot(L, H));
	float dotNH = saturate(dot(N, H));
#endif
	float alphaSqr = alpha * alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH * (alphaSqr - 1.00000) + 1.00000f;
	float D = alphaSqr / (pi * denom * denom);
	float dotLH5 = pow(1.00000f - dotLH, 5);
	float F = F0 + (1.00000 - F0) * (dotLH5);
	float k = alpha / 2.00000f;
	float k2 = k * k;
	float invK2 = 1.00000f - k2;
	float vis = 1.00000 / (dotLH * dotLH * invK2 + k2);
	float specular = dotNL * D * F * vis;
	return specular;
}

float3 calcAmbientColor(float3 upDir, float3 normal)
{
	float3 color = float3(0.000000, 0.000000, 0.000000);
	float NoU = dot(normal, upDir);
	color.xyz = color.xyz + skyLightColor * max(NoU + 1, 0.000000) / 2.00000;
	color.xyz = color.xyz + groundLightColor * max(-NoU + 1, 0.000000) / 2.00000;
	return color;
}

float3 calcDirectionalLightDiffuseColor(float3 diffuseColor, float3 normal, float3 lightDir, float shadow, float ao)
{
	float3 color = float3(0.000000, 0.000000, 0.000000);
	float NoL = dot(normal, lightDir);
	color.xyz = directionalLightColor * max(NoL, 0.000000) * shadow * ao;
	color.xyz = color.xyz * diffuseColor.xyz;
	return color;
}

float3 calcDirectionalLightSpecularColor(float3 specularColor, float3 normal, float3 lightDir, float smoothness, float fresnel, float shadow, float ao)
{
	float3 viewDir = float3(0.000000, 0.000000, 1.00000);
	float specular = calcLightingGGX(normal, viewDir, lightDir, smoothness, fresnel);
	specular = specular * shadow * ao;
	float3 color = float3(specular, specular, specular);
	return color * specularColor;
}

float VSM(float2 moments, float t)
{
	float ex = moments.x;
	float ex2 = moments.y;
	float p = 0.000000;
	if(t <= ex) p = 1.00000;
	float variance = ex2 - ex * ex;
	variance = max(variance, 2.00000e-005);
	float d = t - ex;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}



//||>

float3 GetDiffuseColor(float2 uv)
{
	return g_gbuffer0Texture.Sample(g_gbuffer0Sampler, uv).xyz;
}

float4 GetSpecularColorAndSmoothness(float2 uv)
{
	return g_gbuffer1Texture.Sample(g_gbuffer1Sampler, uv).xyzw;
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
	return reconstructInfo1.x / (reconstructInfo1.y * z + reconstructInfo1.z);
	//return -((z * reconstructInfo1.x) + reconstructInfo1.y);
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

	float3 diffuseColor = GetDiffuseColor(uv);
	float3 normal = GetNormal(uv);
	float4 specularColorAndSmoothness = GetSpecularColorAndSmoothness(uv);
	float3 specularColor = specularColorAndSmoothness.xyz;
	float smoothness = specularColorAndSmoothness.w;

#ifdef DIRECTIONAL_LIGHT
	float4 shadowmapPos = ReconstructShadowmapPosition(cameraPos);
	float4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos);
	float2 shadowmapUV = GetShadowmapUV(projShadowmapPos);

	float depth = projShadowmapPos.z;
	float2 shadowParam = g_shadowmapTexture.Sample(g_shadowmapSampler, shadowmapUV).xy;

	float shadow = VSM(shadowParam, depth );
	float ao = g_ssaoTexture.Sample(g_ssaoSampler, uv).x;

	lightColor.xyz += calcDirectionalLightDiffuseColor(diffuseColor, normal, directionalLightDirection, shadow, ao);

	lightColor.xyz += calcDirectionalLightSpecularColor(specularColor, normal, directionalLightDirection, smoothness, 0.06, shadow, ao);
#endif

#ifdef AMBIENT_LIGHT
	lightColor.xyz += calcAmbientColor(upDir, normal) * diffuseColor;
	lightColor.xyz += g_environmentDiffuseTexture.Sample(g_environmentDiffuseSampler, uv).xyz * diffuseColor;
#endif

	return lightColor;
}

)";
