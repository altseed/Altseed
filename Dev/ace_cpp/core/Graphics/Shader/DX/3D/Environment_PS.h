static const char* environment_ps_dx = R"(

TextureCube		g_diffuseTexture;
SamplerState	g_diffuseSampler;

TextureCube		g_specularTexture;
SamplerState	g_specularSampler;

Texture2D		g_gbuffer0Texture;
SamplerState	g_gbuffer0Sampler;

Texture2D		g_gbuffer1Texture;
SamplerState	g_gbuffer1Sampler;

Texture2D		g_gbuffer2Texture;
SamplerState	g_gbuffer2Sampler;

Texture2D		g_gbuffer3Texture;
SamplerState	g_gbuffer3Sampler;

float3 reconstructInfo1;
float4 reconstructInfo2;

float3		frontDir;
float3		rightDir;
float3		upDir;

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

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

float4 main( const PS_Input Input ) : SV_Target
{
	float2 uv = Input.UV;
	// uv.y = 1.0 - uv.y;

	float3 normal = GetNormal(uv);

	float3 globalNormal = rightDir * normal.x + upDir * normal.y + frontDir * normal.z;
	float4 diffuseColor = g_diffuseTexture.Sample(g_diffuseSampler, globalNormal);

	float4 Output = diffuseColor;
	
	//Output.xyz = globalNormal;
	Output.w = 1.0;

	return Output;
}

)";
