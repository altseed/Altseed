static const char* light_ps_gl = R"(

uniform sampler2D		g_gbuffer0Texture;

uniform sampler2D		g_gbuffer1Texture;

uniform sampler2D		g_gbuffer2Texture;

uniform sampler2D		g_gbuffer3Texture;

uniform sampler2D		g_ssaoTexture;

uniform sampler2D		g_environmentDiffuseTexture;

#ifdef __CASCATED__
uniform sampler2D		g_shadowmap0Texture;
uniform int				g_shadowmap0Sampler;
uniform sampler2D		g_shadowmap1Texture;
uniform int				g_shadowmap1Sampler;
uniform sampler2D		g_shadowmap2Texture;
uniform int				g_shadowmap2Sampler;
#else
uniform sampler2D		g_shadowmapTexture;
#endif

#ifdef __CASCATED__
uniform mat4		g_cameraPositionToShadowCameraPosition0;
uniform mat4		g_shadowProjection0;
uniform mat4		g_cameraPositionToShadowCameraPosition1;
uniform mat4		g_shadowProjection1;
uniform mat4		g_cameraPositionToShadowCameraPosition2;
uniform mat4		g_shadowProjection2;

uniform vec4			g_threshold;
uniform vec4			g_shadowScale;

#else
uniform mat4			g_cameraPositionToShadowCameraPosition;
uniform mat4			g_shadowProjection;
#endif

uniform vec3			reconstructInfo1;
uniform vec4			reconstructInfo2;

uniform vec3			directionalLightDirection;
uniform vec3			directionalLightColor;
uniform vec3			skyLightColor;
uniform vec3			groundLightColor;
uniform vec3			upDir;

in vec4 voutPosition;
in vec2 voutUV;

out vec4 outOutput0;


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
	float dotLH5 = pow(1.00000 - dotLH, 5);
	return F0 + (1.00000 - F0) * (dotLH5);
}

float calcG_Schlick(float roughness, float dotNV, float dotNL)
{
	float k = (roughness + 1.00000) * (roughness + 1.00000) / 8.00000;
	float gV = dotNV * (1.00000 - k) + k;
	float gL = dotNL * (1.00000 - k) + k;
	return 1.00000 / (gV * gL);
}

float calcLightingGGX(vec3 N, vec3 V, vec3 L, float roughness, float F0)
{
	float alpha = roughness * roughness;
	vec3 H = normalize(V + L);
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

vec3 calcAmbientColor(vec3 upDir, vec3 normal)
{
	vec3 color = vec3(0.000000, 0.000000, 0.000000);
	float NoU = dot(normal, upDir);
	color.xyz = color.xyz + skyLightColor * max(NoU + 1, 0.000000) / 2.00000;
	color.xyz = color.xyz + groundLightColor * max(-NoU + 1, 0.000000) / 2.00000;
	return color;
}

vec3 calcDirectionalLightDiffuseColor(vec3 diffuseColor, vec3 normal, vec3 lightDir, float shadow, float ao)
{
	vec3 color = vec3(0.000000, 0.000000, 0.000000);
	float NoL = dot(normal, lightDir);
	color.xyz = directionalLightColor * max(NoL, 0.000000) * shadow * ao / 3.14000;
	color.xyz = color.xyz * diffuseColor.xyz;
	return color;
}

vec3 calcDirectionalLightSpecularColor(vec3 specularColor, vec3 normal, vec3 viewDir, vec3 lightDir, float smoothness, float fresnel, float shadow, float ao)
{
	float roughness = 1.00000 - smoothness;
	roughness = max(roughness, 0.08);
	roughness = min(roughness, 0.92);

	vec3 specular;
	specular.x = calcLightingGGX(normal, viewDir, lightDir, roughness, specularColor.x);
	specular.y = calcLightingGGX(normal, viewDir, lightDir, roughness, specularColor.y);
	specular.z = calcLightingGGX(normal, viewDir, lightDir, roughness, specularColor.z);
	specular = specular * shadow * ao;
	specular.xyz = directionalLightColor * specular.xyz;
	float NoL = dot(normal, lightDir);
	specular.xyz = specular.xyz * max(NoL, 0.000000);
	return specular;
}

float VSM(vec2 moments, float t)
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

vec3 CalcDiffuseColor(vec3 baseColor, float metalness)
{
	return baseColor * (1.00000 - metalness);
}

vec3 CalcSpecularColor(vec3 baseColor, float metalness)
{
	vec3 minColor = vec3(0.0400000, 0.0400000, 0.0400000);
	return minColor.xyz * (1.00000 - metalness) + baseColor.xyz * metalness;
}





//||>


vec4 GetBaseColor(vec2 uv)
{
	return texture(g_gbuffer0Texture, uv).xyzw;
}

vec4 GetSmoothnessMetalnessAO(vec2 uv)
{
	return texture(g_gbuffer1Texture, uv).xyzw;
}

vec3 GetNormal(vec2 uv)
{
	return texture(g_gbuffer2Texture, uv).xyz;
}

vec3 GetAO(vec2 uv)
{
	float ao = texture(g_ssaoTexture, uv).x;
	return vec3(ao,ao,ao);
}

float GetNormalizedDepth(vec2 uv)
{
	return texture(g_gbuffer2Texture, uv).w;
}

float ReconstructDepth(float z)
{
	return reconstructInfo1.x / (reconstructInfo1.y * z + reconstructInfo1.z);
}

vec3 ReconstructPosition(vec2 screenXY, float depth)
{
	return vec3( reconstructInfo2.xy * screenXY * (-depth), depth );
}

#ifdef __CASCATED__
vec4 ReconstructShadowmapPosition(vec3 cameraPos, mat4 cameraPositionToShadowCameraPosition)
{
	vec4 shadowmapPos = cameraPositionToShadowCameraPosition * vec4(cameraPos,1.0);
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

vec4 ReconstructProjectedShadowmapPosition(vec4 shadowmapPosition, mat4 shadowProjection)
{
	vec4 shadowmapPos = shadowProjection * shadowmapPosition;
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

#else

vec4 ReconstructShadowmapPosition(vec3 cameraPos)
{
	vec4 shadowmapPos = g_cameraPositionToShadowCameraPosition * vec4(cameraPos,1.0);
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

vec4 ReconstructProjectedShadowmapPosition(vec4 shadowmapPosition)
{
	vec4 shadowmapPos = g_shadowProjection * shadowmapPosition;
	shadowmapPos = shadowmapPos / shadowmapPos.w;
	return shadowmapPos;
}

#endif

vec2 GetShadowmapUV(vec4 shadowmapPos)
{
	return vec2( (shadowmapPos.x + 1.0) / 2.0f, (shadowmapPos.y + 1.0) / 2.0f );
}

vec4 SampleTexture(sampler2D texture_, int sampler_, vec2 uv_){
	return texture(texture_, uv_);
}
uvec2 GetTextureSize(sampler2D texture_, int sampler_){
	return uvec2(textureSize(texture_, 0));
}

float CalcShadow(float shadowMapDepth, float depth, float scale)
{
	return max(min(1.0 + ( shadowMapDepth - depth) * scale, 1.0),0.0);
}

vec4 CalcShadow(vec4 shadowMapDepth, float depth, float s)
{
	return vec4(
		CalcShadow(shadowMapDepth.x, depth, s),
		CalcShadow(shadowMapDepth.y, depth, s),
		CalcShadow(shadowMapDepth.z, depth, s),
		CalcShadow(shadowMapDepth.w, depth, s));
}

vec4 Gather22(sampler2D tex_, int sp_, vec2 uv_, vec2 itv)
{
	//return tex_.Gather(sp_, uv_);

	return vec4(
		SampleTexture(tex_, sp_, uv_ + itv * vec2(0.0,1.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(1.0,1.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(1.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(0.0,0.0)).x);
}

vec4 Gather14(sampler2D tex_, int sp_, vec2 uv_, vec2 itv)
{
	return vec4(
		SampleTexture(tex_, sp_, uv_ + itv * vec2(0.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(0.0,1.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(0.0,2.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(0.0,3.0)).x);
}

vec4 Gather41(sampler2D tex_, int sp_, vec2 uv_, vec2 itv)
{
	return vec4(
		SampleTexture(tex_, sp_, uv_ + itv * vec2(0.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(1.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(2.0,0.0)).x,
		SampleTexture(tex_, sp_, uv_ + itv * vec2(3.0,0.0)).x);
}

float Sum(vec4 v)
{
	return v.x + v.y + v.z + v.w;
}

float PCF1x1(vec2 uv, float depth, sampler2D tex_, int sp_, float s)
{
	float value = SampleTexture(tex_, sp_, uv).x;
	return CalcShadow(value, depth, s);
}

float PCF2x2(vec2 uv, float depth, sampler2D tex_, int sp_, float s)
{
	uvec2 textureSize = GetTextureSize(tex_, sp_);
	vec2 texSize = vec2(textureSize);
	vec2 itv = vec2(1.0,1.0) / texSize;

	uv = uv + itv * vec2(-0.5,-0.5);

	vec4 values = Gather22(tex_, sp_, uv, itv);
	vec4 shadows = CalcShadow(values, depth, s);
	return Sum(shadows) / 4.0;
}

float PCF3x3(vec2 uv, float depth, sampler2D tex_, int sp_, float s)
{
	uvec2 textureSize = GetTextureSize(tex_, sp_);
	vec2 texSize = vec2(textureSize);
	vec2 itv = vec2(1.0 / float(textureSize.x),1.0 / float(textureSize.y));

	vec2 uv00_11 = uv + itv * vec2(-1.0,-1.0);
	vec4 values00_11 = Gather22(tex_, sp_, uv00_11, itv);
	vec4 shadows00_11 = CalcShadow(values00_11, depth, s);

	float value20 = SampleTexture(tex_, sp_, uv + itv * vec2(+1.0,-1.0)).x;
	float value21 = SampleTexture(tex_, sp_, uv + itv * vec2(+1.0,+0.0)).x;
	float value02 = SampleTexture(tex_, sp_, uv + itv * vec2(-1.0,+1.0)).x;
	float value12 = SampleTexture(tex_, sp_, uv + itv * vec2(+0.0,+1.0)).x;
	float value22 = SampleTexture(tex_, sp_, uv + itv * vec2(+1.0,+1.0)).x;

	return (
Sum(shadows00_11)+
CalcShadow(value20, depth, s)+
CalcShadow(value21, depth, s)+
CalcShadow(value02, depth, s)+
CalcShadow(value12, depth, s)+
CalcShadow(value22, depth, s) ) / 9.0;
}

float PCF4x4(vec2 uv, float depth, sampler2D tex_, int sp_, float s)
{
	uvec2 textureSize = GetTextureSize(tex_, sp_);
	vec2 texSize = vec2(textureSize);
	vec2 itv = vec2(1.0 / float(textureSize.x),1.0 / float(textureSize.y));

	uv = uv + itv * vec2(-0.5,-0.5);

	vec2 uv00_11 = uv + itv * vec2(-1.0,-1.0);
	vec4 values00_11 = Gather22(tex_, sp_, uv00_11, itv);
	vec4 shadows00_11 = CalcShadow(values00_11, depth, s);

	vec2 uv20_31 = uv + itv * vec2(+1.0,-1.0);
	vec4 values20_31 = Gather22(tex_, sp_, uv20_31, itv);
	vec4 shadows20_31 = CalcShadow(values20_31, depth, s);

	vec2 uv02_13 = uv + itv * vec2(-1.0,+1.0);
	vec4 values02_13 = Gather22(tex_, sp_, uv02_13, itv);
	vec4 shadows02_13 = CalcShadow(values02_13, depth, s);

	vec2 uv22_33 = uv + itv * vec2(+1.0,+1.0);
	vec4 values22_33 = Gather22(tex_, sp_, uv22_33, itv);
	vec4 shadows22_33 = CalcShadow(values22_33, depth, s);

	return ( Sum(shadows00_11)+Sum(shadows20_31)+Sum(shadows02_13)+Sum(shadows22_33)) / 16.0;
}

float PCF5x5(vec2 uv, float depth, sampler2D tex_, int sp_, float s)
{
	uvec2 textureSize = GetTextureSize(tex_, sp_);
	vec2 texSize = vec2(textureSize);
	vec2 itv = vec2(1.0 / float(textureSize.x),1.0 / float(textureSize.y));

	vec2 uv_pcf44 = uv + itv * vec2(-0.5,-0.5);
	float pcf44 = PCF4x4(uv_pcf44, depth, tex_, sp_, s) * 16.0;
	
	vec2 uv41 = uv + itv * vec2(-2.0,+2.0);
	vec4 values41 = Gather41(tex_, sp_, uv41, itv);
	vec4 shadows41 = CalcShadow(values41, depth, s);

	vec2 uv14 = uv + itv * vec2(+2.0,-2.0);
	vec4 values14 = Gather14(tex_, sp_, uv14, itv);
	vec4 shadows14 = CalcShadow(values14, depth, s);

	vec2 uv44 = uv + itv * vec2(+2.0,+2.0);
	float values44 = SampleTexture(tex_, sp_, uv44).x;
	float shadows44 = CalcShadow(values44, depth, s);

	return ( Sum(shadows41)+Sum(shadows14)+pcf44+shadows44) / 25.0;
}

#ifdef __CASCATED__
float CalcShadow0(vec3 cameraPos)
{
	vec4 shadowmapPos = ReconstructShadowmapPosition(cameraPos, g_cameraPositionToShadowCameraPosition0);
	vec4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos, g_shadowProjection0);
	vec2 shadowmapUV = GetShadowmapUV(projShadowmapPos);
	
	float depth = projShadowmapPos.z - 0.01;
	vec2 center = shadowmapUV.xy;

	// GL
	depth = depth * 0.5 + 0.5;

	return PCF5x5(center, depth, g_shadowmap0Texture, g_shadowmap0Sampler, g_shadowScale.x);
}

float CalcShadow1(vec3 cameraPos)
{
	vec4 shadowmapPos = ReconstructShadowmapPosition(cameraPos, g_cameraPositionToShadowCameraPosition1);
	vec4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos, g_shadowProjection1);
	vec2 shadowmapUV = GetShadowmapUV(projShadowmapPos);
	
	float depth = projShadowmapPos.z - 0.01;
	vec2 center = shadowmapUV.xy;

	// GL
	depth = depth * 0.5 + 0.5;

	return PCF3x3(center, depth, g_shadowmap1Texture, g_shadowmap1Sampler, g_shadowScale.y);
}

float CalcShadow2(vec3 cameraPos)
{
	vec4 shadowmapPos = ReconstructShadowmapPosition(cameraPos, g_cameraPositionToShadowCameraPosition2);
	vec4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos, g_shadowProjection2);
	vec2 shadowmapUV = GetShadowmapUV(projShadowmapPos);
	
	float depth = projShadowmapPos.z - 0.01;
	vec2 center = shadowmapUV.xy;

	// GL
	depth = depth * 0.5 + 0.5;

	return PCF1x1(center, depth, g_shadowmap2Texture, g_shadowmap2Sampler, g_shadowScale.z);
}

#endif

void main()
{
	vec2 uv = voutUV;
	uv.y = 1.0 - uv.y;

	vec3 cameraPos = ReconstructPosition(voutPosition.xy, ReconstructDepth(GetNormalizedDepth(uv)));

	vec3 viewDir = normalize(-cameraPos);

	vec4 lightColor = vec4(0.0,0.0,0.0,1.0);

	vec4 baseColor_ = GetBaseColor(uv);
	if(baseColor_.a == 0.0f) discard;

	vec3 baseColor = baseColor_.xyz;
	vec3 normal = GetNormal(uv);
	vec4 smoothnessMetalnessAO = GetSmoothnessMetalnessAO(uv);
	float smoothness = smoothnessMetalnessAO .x;
	float metalness = smoothnessMetalnessAO .y;
	float bakedAO = smoothnessMetalnessAO .z;

	vec3 diffuseColor = CalcDiffuseColor(baseColor,metalness);
	vec3 specularColor = CalcSpecularColor(baseColor,metalness);

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
	vec4 shadowmapPos = ReconstructShadowmapPosition(cameraPos);
	vec4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos);
	vec2 shadowmapUV = GetShadowmapUV(projShadowmapPos);

	float depth = projShadowmapPos.z;
	
	// GL
	depth = depth * 0.5 + 0.5;

	vec2 shadowParam = texture(g_shadowmapTexture, shadowmapUV).xy;

	float shadow = VSM(shadowParam, depth);
	lightColor.xyz += calcDirectionalLightDiffuseColor(diffuseColor, normal, directionalLightDirection, shadow, ao);

	lightColor.xyz += calcDirectionalLightSpecularColor(specularColor, normal, viewDir, directionalLightDirection, smoothness, 0.06, shadow, ao);
#endif

#endif

#ifdef AMBIENT_LIGHT
	lightColor.xyz += calcAmbientColor(upDir, normal) * diffuseColor * ao;
	lightColor.xyz += texture(g_environmentDiffuseTexture, uv).xyz * ao;
#endif

	outOutput0 = lightColor;
}

)";
