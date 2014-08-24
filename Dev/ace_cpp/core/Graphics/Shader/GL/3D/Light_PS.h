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
float calcLightingGGX(vec3 N, vec3 V, vec3 L, float roughness, float F0)
{
	float alpha = roughness * roughness;
	vec3 H = normalize(V + L);
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
	float pi = 3.14159;
	float denom = dotNH * dotNH * (alphaSqr - 1.00000) + 1.00000;
	float D = alphaSqr / (pi * denom * denom);
	float dotLH5 = pow(1.00000 - dotLH, 5);
	float F = F0 + (1.00000 - F0) * (dotLH5);
	float k = alpha / 2.00000;
	float k2 = k * k;
	float invK2 = 1.00000 - k2;
	float vis = 1.00000 / (dotLH * dotLH * invK2 + k2);
	float specular = dotNL * D * F * vis;
	return specular;
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
	color.xyz = directionalLightColor * max(NoL, 0.000000) * shadow * ao;
	color.xyz = color.xyz * diffuseColor.xyz;
	return color;
}

vec3 calcDirectionalLightSpecularColor(vec3 specularColor, vec3 normal, vec3 lightDir, float smoothness, float fresnel, float shadow, float ao)
{
	vec3 viewDir = vec3(0.000000, 0.000000, 1.00000);
	float specular = calcLightingGGX(normal, viewDir, lightDir, smoothness, fresnel);
	specular = specular * shadow * ao;
	vec3 color = vec3(specular, specular, specular);
	return color * specularColor;
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




//||>

vec3 GetDiffuseColor(vec2 uv)
{
	return texture(g_gbuffer0Texture, uv).xyz;
}

vec4 GetSpecularColorAndSmoothness(vec2 uv)
{
	return texture(g_gbuffer1Texture, uv).xyzw;
}

vec3 GetNormal(vec2 uv)
{
	return texture(g_gbuffer2Texture, uv).xyz;
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

	vec4 lightColor = vec4(0.0,0.0,0.0,1.0);
	vec3 normal = GetNormal(uv);

	vec3 diffuseColor = GetDiffuseColor(uv);
	vec4 specularColorAndSmoothness = GetSpecularColorAndSmoothness(uv);
	vec3 specularColor = specularColorAndSmoothness.xyz;
	float smoothness = specularColorAndSmoothness.w;

#ifdef DIRECTIONAL_LIGHT
	vec4 shadowmapPos = ReconstructShadowmapPosition(cameraPos);
	vec4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos);
	vec2 shadowmapUV = GetShadowmapUV(projShadowmapPos);

	float depth = projShadowmapPos.z;
	
	// GL
	depth = depth * 0.5 + 0.5;

	vec2 shadowParam = texture(g_shadowmapTexture, shadowmapUV).xy;

	float shadow = VSM(shadowParam, depth);
	float ao = texture(g_ssaoTexture, uv).x;

	lightColor.xyz += calcDirectionalLightDiffuseColor(diffuseColor, normal, directionalLightDirection, shadow, ao);

	lightColor.xyz += calcDirectionalLightSpecularColor(specularColor, normal, directionalLightDirection, smoothness, 0.06, shadow, ao);
#endif

#ifdef AMBIENT_LIGHT
	lightColor.xyz += calcAmbientColor(upDir, normal) * diffuseColor;
	lightColor.xyz += texture(g_environmentDiffuseTexture, uv).xyz * diffuseColor;
#endif

	outOutput0 = lightColor;
}

)";
