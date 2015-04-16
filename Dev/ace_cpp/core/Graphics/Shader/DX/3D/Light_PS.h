static const char* light_ps_dx = R"(

Texture2D		g_gbuffer0Texture		: register( t0 );
SamplerState	g_gbuffer0Sampler		: register( s0 );

Texture2D		g_gbuffer1Texture		: register( t1 );
SamplerState	g_gbuffer1Sampler		: register( s1 );

Texture2D		g_gbuffer2Texture		: register( t2 );
SamplerState	g_gbuffer2Sampler		: register( s2 );

Texture2D		g_gbuffer3Texture		: register( t3 );
SamplerState	g_gbuffer3Sampler		: register( s3 );

Texture2D		g_ssaoTexture		: register( t4 );
SamplerState	g_ssaoSampler		: register( s4 );

#ifdef __CASCATED__
Texture2D		g_shadowmap0Texture		: register( t5 );
SamplerState	g_shadowmap0Sampler		: register( s5 );
Texture2D		g_shadowmap1Texture		: register( t6 );
SamplerState	g_shadowmap1Sampler		: register( s6 );
Texture2D		g_shadowmap2Texture		: register( t7 );
SamplerState	g_shadowmap2Sampler		: register( s7 );
#else
Texture2D		g_shadowmapTexture		: register( t5 );
SamplerState	g_shadowmapSampler		: register( s5 );
#endif



Texture2D		g_environmentDiffuseTexture;
SamplerState	g_environmentDiffuseSampler;

#ifdef __CASCATED__
float4x4		g_cameraPositionToShadowCameraPosition0;
float4x4		g_shadowProjection0;
float4x4		g_cameraPositionToShadowCameraPosition1;
float4x4		g_shadowProjection1;
float4x4		g_cameraPositionToShadowCameraPosition2;
float4x4		g_shadowProjection2;

float4			g_threshold;
float4			g_shadowScale;

#else
float4x4		g_cameraPositionToShadowCameraPosition;
float4x4		g_shadowProjection;
#endif

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
float calcD_GGX(float roughness, float dotNH)
{
	float alpha = roughness * roughness;
	float alphaSqr = alpha * alpha;
	float pi = 3.14159;
	float denom = dotNH * dotNH * (alphaSqr - 1.00000) + 1.00000;
	return (alpha / denom) * (alpha / denom) / pi;
}

float calcF(float F0, float dotLH)
{
	float dotLH5 = pow(1.00000f - dotLH, 5);
	return F0 + (1.00000 - F0) * (dotLH5);
}

float calcG_Schlick(float roughness, float dotNV, float dotNL)
{
	float k = (roughness + 1.00000) * (roughness + 1.00000) / 8.00000;
	float gV = dotNV * (1.00000 - k) + k;
	float gL = dotNL * (1.00000 - k) + k;
	return 1.00000 / (gV * gL);
}

float calcLightingGGX(float3 N, float3 V, float3 L, float roughness, float F0)
{
	float alpha = roughness * roughness;
	float3 H = normalize(V + L);
#ifdef OPENGL
	float dotNL = clamp(dot(N, L), 0.000000, 1.00000);
	float dotLH = clamp(dot(L, H), 0.000000, 1.00000);
	float dotNH = clamp(dot(N, H), 0.000000, 1.00000) - 0.001;
	float dotNV = clamp(dot(N, V), 0.000000, 1.00000) + 0.001;
#endif
#ifdef DIRECTX
	float dotNL = saturate(dot(N, L));
	float dotLH = saturate(dot(L, H));
	float dotNH = saturate(dot(N, H)) - 0.001;
	float dotNV = saturate(dot(N, V)) + 0.001;
#endif
	float D = calcD_GGX(roughness, dotNH);
	float F = calcF(F0, dotLH);
	float G = calcG_Schlick(roughness, dotNV, dotNL);

	return dotNL * D * F * G / 4.00000;
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
	color.xyz = directionalLightColor * max(NoL, 0.000000) * shadow * ao / 3.14000;
	color.xyz = color.xyz * diffuseColor.xyz;
	return color;
}

float3 calcDirectionalLightSpecularColor(float3 specularColor, float3 normal, float3 viewDir, float3 lightDir, float smoothness, float fresnel, float shadow, float ao)
{
	float roughness = 1.00000 - smoothness;
	roughness = max(roughness, 0.08);
	roughness = min(roughness, 0.92);

	float3 specular;
	specular.x = calcLightingGGX(normal, viewDir, lightDir, roughness, specularColor.x);
	specular.y = calcLightingGGX(normal, viewDir, lightDir, roughness, specularColor.y);
	specular.z = calcLightingGGX(normal, viewDir, lightDir, roughness, specularColor.z);
	specular = specular * shadow * ao;
	specular.xyz = directionalLightColor * specular.xyz;
	float NoL = dot(normal, lightDir);
	specular.xyz = specular.xyz * max(NoL, 0.000000);
	return specular;
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

float3 CalcDiffuseColor(float3 baseColor, float metalness)
{
	return baseColor * (1.00000 - metalness);
}

float3 CalcSpecularColor(float3 baseColor, float metalness)
{
	float3 minColor = float3(0.0400000, 0.0400000, 0.0400000);
	return minColor.xyz * (1.00000 - metalness) + baseColor.xyz * metalness;
}






//||>

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

float3 GetAO(float2 uv)
{
	float ao = g_ssaoTexture.Sample(g_ssaoSampler, uv).x;
	return float3(ao,ao,ao);
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

#ifdef __CASCATED__
float4 ReconstructShadowmapPosition(float3 cameraPos, float4x4 cameraPositionToShadowCameraPosition)
{
	float4 shadowmapPos = mul( cameraPositionToShadowCameraPosition, float4(cameraPos,1.0) );
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

float4 ReconstructProjectedShadowmapPosition(float4 shadowmapPosition, float4x4 shadowProjection)
{
	float4 shadowmapPos = mul( shadowProjection, shadowmapPosition );
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

#else
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
#endif

float2 GetShadowmapUV(float4 shadowmapPos)
{
	return float2( (shadowmapPos.x + 1.0) / 2.0f, 1.0 - (shadowmapPos.y + 1.0) / 2.0f );
}

float4 SampleTexture(Texture2D texture_, SamplerState sampler_, float2 uv_) {
	return texture_.Sample(sampler_, uv_);
}

uint2 GetTextureSize(Texture2D texture_, SamplerState sampler_){
	uint width, height;
	texture_.GetDimensions(width, height);
	return uint2(width, height);
}

float CalcShadow(float shadowMapDepth, float depth, float scale)
{
	return saturate(1.0 + ( shadowMapDepth - depth) * scale );
}

float4 CalcShadow(float4 shadowMapDepth, float depth, float s)
{
	return float4(
		CalcShadow(shadowMapDepth.x, depth, s),
		CalcShadow(shadowMapDepth.y, depth, s),
		CalcShadow(shadowMapDepth.z, depth, s),
		CalcShadow(shadowMapDepth.w, depth, s));
}

float4 Gather22(Texture2D tex_, SamplerState sp_, float2 uv_, float2 itv)
{
	//return tex_.Gather(sp_, uv_);

	return float4(
		SampleTexture(tex_, sp_, uv_ + itv * float2(0.0,1.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(1.0,1.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(1.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(0.0,0.0)).x);
}

float4 Gather14(Texture2D tex_, SamplerState sp_, float2 uv_, float2 itv)
{
	return float4(
		SampleTexture(tex_, sp_, uv_ + itv * float2(0.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(0.0,1.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(0.0,2.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(0.0,3.0)).x);
}

float4 Gather41(Texture2D tex_, SamplerState sp_, float2 uv_, float2 itv)
{
	return float4(
		SampleTexture(tex_, sp_, uv_ + itv * float2(0.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(1.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(2.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * float2(3.0,0.0)).x);
}

float Sum(float4 v)
{
	return v.x + v.y + v.z + v.w;
}

float PCF1x1(float2 uv, float depth, Texture2D tex_, SamplerState sp_, float s)
{
	float value = SampleTexture(tex_, sp_, uv).x;
	return CalcShadow(value, depth, s);
}

float PCF2x2(float2 uv, float depth, Texture2D tex_, SamplerState sp_, float s)
{
	uint2 textureSize = GetTextureSize(tex_, sp_);
	float2 texSize = float2(textureSize);
	float2 itv = float2(1.0,1.0) / texSize;

	uv = uv + itv * float2(-0.5,-0.5);

	float4 values = Gather22(tex_, sp_, uv, itv);
	float4 shadows = CalcShadow(values, depth, s);
	return Sum(shadows) / 4.0;
}

float PCF3x3(float2 uv, float depth, Texture2D tex_, SamplerState sp_, float s)
{
	uint2 textureSize = GetTextureSize(tex_, sp_);
	float2 texSize = float2(textureSize);
	float2 itv = float2(1.0 / float(textureSize.x),1.0 / float(textureSize.y));

	float2 uv00_11 = uv + itv * float2(-1.0,-1.0);
	float4 values00_11 = Gather22(tex_, sp_, uv00_11, itv);
	float4 shadows00_11 = CalcShadow(values00_11, depth, s);

	float value20 = SampleTexture(tex_, sp_, uv + itv * float2(+1.0,-1.0)).x;
	float value21 = SampleTexture(tex_, sp_, uv + itv * float2(+1.0,+0.0)).x;
	float value02 = SampleTexture(tex_, sp_, uv + itv * float2(-1.0,+1.0)).x;
	float value12 = SampleTexture(tex_, sp_, uv + itv * float2(+0.0,+1.0)).x;
	float value22 = SampleTexture(tex_, sp_, uv + itv * float2(+1.0,+1.0)).x;

	return (
Sum(shadows00_11)+
CalcShadow(value20, depth, s)+
CalcShadow(value21, depth, s)+
CalcShadow(value02, depth, s)+
CalcShadow(value12, depth, s)+
CalcShadow(value22, depth, s) ) / 9.0;
}

float PCF4x4(float2 uv, float depth, Texture2D tex_, SamplerState sp_, float s)
{
	uint2 textureSize = GetTextureSize(tex_, sp_);
	float2 texSize = float2(textureSize);
	float2 itv = float2(1.0 / float(textureSize.x),1.0 / float(textureSize.y));

	uv = uv + itv * float2(-0.5,-0.5);

	float2 uv00_11 = uv + itv * float2(-1.0,-1.0);
	float4 values00_11 = Gather22(tex_, sp_, uv00_11, itv);
	float4 shadows00_11 = CalcShadow(values00_11, depth, s);

	float2 uv20_31 = uv + itv * float2(+1.0,-1.0);
	float4 values20_31 = Gather22(tex_, sp_, uv20_31, itv);
	float4 shadows20_31 = CalcShadow(values20_31, depth, s);

	float2 uv02_13 = uv + itv * float2(-1.0,+1.0);
	float4 values02_13 = Gather22(tex_, sp_, uv02_13, itv);
	float4 shadows02_13 = CalcShadow(values02_13, depth, s);

	float2 uv22_33 = uv + itv * float2(+1.0,+1.0);
	float4 values22_33 = Gather22(tex_, sp_, uv22_33, itv);
	float4 shadows22_33 = CalcShadow(values22_33, depth, s);

	return ( Sum(shadows00_11)+Sum(shadows20_31)+Sum(shadows02_13)+Sum(shadows22_33)) / 16.0;
}

float PCF5x5(float2 uv, float depth, Texture2D tex_, SamplerState sp_, float s)
{
	uint2 textureSize = GetTextureSize(tex_, sp_);
	float2 texSize = float2(textureSize);
	float2 itv = float2(1.0 / float(textureSize.x),1.0 / float(textureSize.y));

	float2 uv_pcf44 = uv + itv * float2(-0.5,-0.5);
	float pcf44 = PCF4x4(uv_pcf44, depth, tex_, sp_, s) * 16.0;
	
	float2 uv41 = uv + itv * float2(-2.0,+2.0);
	float4 values41 = Gather41(tex_, sp_, uv41, itv);
	float4 shadows41 = CalcShadow(values41, depth, s);

	float2 uv14 = uv + itv * float2(+2.0,-2.0);
	float4 values14 = Gather14(tex_, sp_, uv14, itv);
	float4 shadows14 = CalcShadow(values14, depth, s);

	float2 uv44 = uv + itv * float2(+2.0,+2.0);
	float values44 = SampleTexture(tex_, sp_, uv44).x;
	float shadows44 = CalcShadow(values44, depth, s);

	return ( Sum(shadows41)+Sum(shadows14)+pcf44+shadows44) / 25.0;
}

#ifdef __CASCATED__
float CalcShadow0(float3 cameraPos)
{
	float4 shadowmapPos = ReconstructShadowmapPosition(cameraPos, g_cameraPositionToShadowCameraPosition0);
	float4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos, g_shadowProjection0);
	float2 shadowmapUV = GetShadowmapUV(projShadowmapPos);
	
	float depth = projShadowmapPos.z - 0.01;
	float2 center = shadowmapUV.xy;

	return PCF5x5(center, depth, g_shadowmap0Texture, g_shadowmap0Sampler, g_shadowScale.x);
}

float CalcShadow1(float3 cameraPos)
{
	float4 shadowmapPos = ReconstructShadowmapPosition(cameraPos, g_cameraPositionToShadowCameraPosition1);
	float4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos, g_shadowProjection1);
	float2 shadowmapUV = GetShadowmapUV(projShadowmapPos);
	
	float depth = projShadowmapPos.z - 0.01;
	float2 center = shadowmapUV.xy;

	return PCF3x3(center, depth, g_shadowmap1Texture, g_shadowmap1Sampler, g_shadowScale.y);
}

float CalcShadow2(float3 cameraPos)
{
	float4 shadowmapPos = ReconstructShadowmapPosition(cameraPos, g_cameraPositionToShadowCameraPosition2);
	float4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos, g_shadowProjection2);
	float2 shadowmapUV = GetShadowmapUV(projShadowmapPos);
	
	float depth = projShadowmapPos.z - 0.01;
	float2 center = shadowmapUV.xy;

	return PCF1x1(center, depth, g_shadowmap2Texture, g_shadowmap2Sampler, g_shadowScale.z);
}

#endif

float4 main( const PS_Input Input ) : SV_Target
{
	float2 uv = Input.UV;
	// uv.y = 1.0 - uv.y;

	float3 cameraPos = ReconstructPosition(Input.Position.xy, ReconstructDepth(GetNormalizedDepth(uv)));

	float3 viewDir = normalize(-cameraPos);

	float4 lightColor = float4(0.0,0.0,0.0,1.0);

	float3 baseColor = GetBaseColor(uv);
	float3 normal = GetNormal(uv);
	float4 smoothnessMetalnessAO = GetSmoothnessMetalnessAO(uv);
	float smoothness = smoothnessMetalnessAO .x;
	float metalness = smoothnessMetalnessAO .y;
	float bakedAO = smoothnessMetalnessAO .z;

	float3 diffuseColor = CalcDiffuseColor(baseColor,metalness);
	float3 specularColor = CalcSpecularColor(baseColor,metalness);

	float ao = GetAO(uv).x * bakedAO;

#ifdef DIRECTIONAL_LIGHT

#ifdef __CASCATED__
	float shadow = 0.0;
	if(cameraPos.z > -g_threshold.y)
	{
		shadow = CalcShadow0(cameraPos);
	}
	else if(cameraPos.z > -g_threshold.z)
	{
		shadow = CalcShadow1(cameraPos);
	}
	else
	{
		shadow = CalcShadow2(cameraPos);
	}

	lightColor.xyz += calcDirectionalLightDiffuseColor(diffuseColor, normal, directionalLightDirection, shadow, ao);
	
	lightColor.xyz += calcDirectionalLightSpecularColor(specularColor, normal, viewDir, directionalLightDirection, smoothness, 0.06, shadow, ao);
	
#else
	float4 shadowmapPos = ReconstructShadowmapPosition(cameraPos);
	float4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos);
	float2 shadowmapUV = GetShadowmapUV(projShadowmapPos);

	float depth = projShadowmapPos.z;
	float2 shadowParam = g_shadowmapTexture.Sample(g_shadowmapSampler, shadowmapUV).xy;

	float shadow = VSM(shadowParam, depth );

	lightColor.xyz += calcDirectionalLightDiffuseColor(diffuseColor, normal, directionalLightDirection, shadow, ao);

	lightColor.xyz += calcDirectionalLightSpecularColor(specularColor, normal, viewDir, directionalLightDirection, smoothness, 0.06, shadow, ao);
#endif

#endif

#ifdef AMBIENT_LIGHT
	lightColor.xyz += calcAmbientColor(upDir, normal) * diffuseColor * ao;
	lightColor.xyz += g_environmentDiffuseTexture.Sample(g_environmentDiffuseSampler, uv).xyz * ao;
#endif

	return lightColor;
}

)";
