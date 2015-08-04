static const char* heightfog_ps_dx = R"(

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

float4x4		g_cameraPositionToWorldPosition;
float4			g_fogColor;
float4			g_fogParam0;
float4			g_fogParam1;

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

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

float3 ReconstructWorldPosition(float3 cameraPos)
{
	float4 pos = mul(g_cameraPositionToWorldPosition, float4(cameraPos,1.0));
	pos = pos / pos.w;
	return pos.xyz;
}

float4 main( const PS_Input Input ) : SV_Target
{
	float2 uv = Input.UV;
	// uv.y = 1.0 - uv.y;

	float startDistance = g_fogParam0.x;
	float density = g_fogParam0.y;
	float falloff = g_fogParam0.z;

	float3 viewerCameraPos = ReconstructPosition(Input.Position.xy, 0.0);
	float3 objectCameraPos = ReconstructPosition(Input.Position.xy, ReconstructDepth(GetNormalizedDepth(uv)));

	float3 viewerPos = ReconstructWorldPosition(viewerCameraPos);
	float3 objectPos = ReconstructWorldPosition(objectCameraPos);
	
	float3 viewerToObject = objectPos - viewerPos;
	float3 viewerToObjectNorm = normalize(viewerToObject);

	// apply startDistance
	float distance = max(0.0, length(viewerToObject) - startDistance);
	viewerToObject = viewerToObjectNorm * distance;
	viewerPos = objectPos - viewerToObject;

	if(distance == 0.0) discard;

	// fog = density * exp(h*(-falloff))

	// float p = - density * (exp(viewerPos.y *(-falloff)) - exp(objectPos.y *(-falloff))) / (-viewerToObject.y / distance * (-falloff));
	
	float epsilon1 = 0.001;
	float epsilon2 = 0.00001;

	float heightRel = viewerToObject.y;
	if(abs(heightRel) < epsilon1)
	{
		heightRel = epsilon1;
	}

	float falloffRel = heightRel * (-falloff);
	falloffRel = max(falloffRel, epsilon2);

	float p = density * exp(viewerPos.y *(-falloff)) * distance * ((1.0-exp(falloffRel)) / falloffRel);

	p = min(p, 0.0);
	float expp = exp(p);

	return float4(g_fogColor.xyz, min(1.0 - expp, 1.0));
}

)";
