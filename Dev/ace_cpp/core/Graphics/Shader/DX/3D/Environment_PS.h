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

Texture2D		g_brdfTexture;
SamplerState	g_brdfSampler;

float3 reconstructInfo1;
float4 reconstructInfo2;

float3		frontDir;
float3		rightDir;
float3		upDir;

float		mipmapCount;

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
	float3 view = float3(0.0,0.0,1.0);

	float4 specColorAndSmoothness = GetSpecularColorAndSmoothness(uv);
	float3 specColor = specColorAndSmoothness.xyz;
	float smoothness = specColorAndSmoothness.w;
	float roughness = 1.0 - smoothness;

	float3 globalNormal = float3(dot(rightDir,normal), dot(upDir,normal), dot(frontDir,normal));

	float4 diffuseEnvColor = g_diffuseTexture.SampleLevel(g_diffuseSampler, globalNormal, 0.0);

	float NoV = saturate( dot( normal, view) );
	float3 R = 2.0 * NoV * normal - view;
	float3 globalR = float3(dot(rightDir,R), dot(upDir,R), dot(frontDir,R));

	float4 specEnvColor = g_specularTexture.SampleLevel(g_specularSampler, globalR, roughness * mipmapCount);

	float2 brdfColor = g_brdfTexture.Sample(g_brdfSampler, float2(roughness,NoV)).xy;

	float3 spec = specEnvColor.xyz * ( specColor * brdfColor.x + float3(brdfColor.y,brdfColor.y,brdfColor.y) );

	float4 Output = float4(0.0,0.0,0.0,1.0);
	Output.xyz = diffuseEnvColor.xyz + spec;
	//Output.xyz = spec;
	
	//Output.xyz = globalNormal;
	Output.w = 1.0;

	return Output;
}

)";
