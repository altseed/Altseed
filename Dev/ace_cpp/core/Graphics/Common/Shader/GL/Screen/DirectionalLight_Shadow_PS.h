static const char* directonalLight_Shadow_ps_gl = R"(
#version 330

uniform sampler2D		g_gbuffer0Texture;

uniform sampler2D		g_gbuffer1Texture;

uniform sampler2D		g_gbuffer2Texture;

uniform sampler2D		g_gbuffer3Texture;

uniform sampler2D		g_shadowmapTexture;

uniform mat4			g_cameraPositionToShadowCameraPosition;
uniform mat4			g_shadowProjection;

uniform vec3			reconstructInfo1;
uniform vec4			reconstructInfo2;

in vec4 voutPosition;
in vec2 voutUV;

out vec4 outOutput0;

float GetNormalizedDepth(vec2 uv)
{
	return texture2D(g_gbuffer2Texture, uv).w;
}

float ReconstructDepth(float z)
{
	return -((z * reconstructInfo1.x) + reconstructInfo1.y);
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

float VSM(vec2 moments, float t)
{
	float ex = moments.x;
	float ex2 = moments.y;

	float p = 0.0;
	if (t <= ex) p = 1.0;

	float variance = ex2 - ex * ex;
	variance = max(variance, 0.4 / (reconstructInfo1.x * reconstructInfo1.x));
	float d = t - ex;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}

void main()
{
	vec2 uv = voutUV;
	uv.y = 1.0 - uv.y;

	vec3 cameraPos = ReconstructPosition(voutPosition.xy, ReconstructDepth(GetNormalizedDepth(uv)));

	vec4 shadowmapPos = ReconstructShadowmapPosition(cameraPos);
	vec4 projShadowmapPos = ReconstructProjectedShadowmapPosition(shadowmapPos);
	vec2 shadowmapUV = GetShadowmapUV(projShadowmapPos);

	outOutput0 = vec4(0.0,0.0,0.0,1.0);

	float depth = (-shadowmapPos.z - reconstructInfo1.y) / reconstructInfo1.x;
	vec2 shadowParam = texture2D(g_shadowmapTexture, shadowmapUV).xy;

	float shadow = VSM(shadowParam, depth );

	outOutput0.x = shadow;
}

)";
