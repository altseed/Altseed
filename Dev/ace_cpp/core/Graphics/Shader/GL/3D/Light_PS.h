static const char* light_ps_gl = R"(

uniform sampler2D		g_gbuffer0Texture;

uniform sampler2D		g_gbuffer1Texture;

uniform sampler2D		g_gbuffer2Texture;

uniform sampler2D		g_gbuffer3Texture;

uniform sampler2D		g_shadowmapTexture;

uniform sampler2D		g_ssaoTexture;

uniform sampler2D		g_environmentDiffuseTexture;

uniform mat4			g_cameraPositionToShadowCameraPosition;
uniform mat4			g_shadowProjection;

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
	float dotNH = clamp(dot(N, H), 0.000000, 1.00000);
	float dotNV = clamp(dot(N, V), 0.000000, 1.00000);
#endif
#ifdef DIRECTX
	float dotNL = saturate(dot(N, L));
	float dotLH = saturate(dot(L, H));
	float dotNH = saturate(dot(N, H));
	float dotNV = saturate(dot(N, V));
#endif
	float D = calcD_GGX(roughness, dotNH);
	float F = calcF(F0, dotLH);
	float G = calcG_Schlick(roughness, dotNV, dotNL);

	D = min(32.0,D);

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


vec3 GetBaseColor(vec2 uv)
{
	return texture(g_gbuffer0Texture, uv).xyz;
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

vec2 GetShadowmapUV(vec4 shadowmapPos)
{
	return vec2( (shadowmapPos.x + 1.0) / 2.0f, (shadowmapPos.y + 1.0) / 2.0f );
}

void main()
{
	vec2 uv = voutUV;
	uv.y = 1.0 - uv.y;

	vec3 cameraPos = ReconstructPosition(voutPosition.xy, ReconstructDepth(GetNormalizedDepth(uv)));

	vec3 viewDir = normalize(-cameraPos);

	vec4 lightColor = vec4(0.0,0.0,0.0,1.0);

	vec3 baseColor = GetBaseColor(uv);
	vec3 normal = GetNormal(uv);
	vec4 smoothnessMetalnessAO = GetSmoothnessMetalnessAO(uv);
	float smoothness = smoothnessMetalnessAO .x;
	float metalness = smoothnessMetalnessAO .y;
	float bakedAO = smoothnessMetalnessAO .z;

	vec3 diffuseColor = CalcDiffuseColor(baseColor,metalness);
	vec3 specularColor = CalcSpecularColor(baseColor,metalness);

	float ao = GetAO(uv).x * bakedAO;

#ifdef DIRECTIONAL_LIGHT
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

#ifdef AMBIENT_LIGHT
	lightColor.xyz += calcAmbientColor(upDir, normal) * diffuseColor * ao;
	lightColor.xyz += texture(g_environmentDiffuseTexture, uv).xyz * ao;
#endif

	outOutput0 = lightColor;
}

)";
