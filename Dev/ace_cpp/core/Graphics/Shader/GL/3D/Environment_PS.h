static const char* environment_ps_gl = R"(

uniform samplerCube		g_diffuseTexture;
uniform samplerCube		g_specularTexture;

uniform sampler2D		g_gbuffer0Texture;

uniform sampler2D		g_gbuffer1Texture;

uniform sampler2D		g_gbuffer2Texture;

uniform sampler2D		g_gbuffer3Texture;

uniform vec3 reconstructInfo1;
uniform vec4 reconstructInfo2;

uniform vec3		frontDir;
uniform vec3		rightDir;
uniform vec3		upDir;

in vec4 voutPosition;
in vec2 voutUV;

out vec4 outOutput0;

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

vec3 CalcDiffuseColor(vec3 baseColor, float metalness)
{
	return baseColor * (1.00000 - metalness);
}

vec3 CalcSpecularColor(vec3 baseColor, float metalness)
{
	vec3 minColor = vec3(0.0400000, 0.0400000, 0.0400000);
	return minColor.xyz * (1.0-metalness) + baseColor.xyz * metalness;
}

void main()
{
	vec2 uv = voutUV;
	uv.y = 1.0 - uv.y;

	vec3 normal = GetNormal(uv);

	vec3 globalNormal = vec3(dot(rightDir,normal), dot(upDir,normal), dot(frontDir,normal));
	globalNormal.y = -globalNormal.y;

	vec4 diffuseColor = textureLod(g_diffuseTexture, globalNormal, 0.0);

	vec4 Output = diffuseColor;
	
	//Output.xyz = globalNormal;
	Output.w = 1.0;

	outOutput0 = Output;
}

)";
