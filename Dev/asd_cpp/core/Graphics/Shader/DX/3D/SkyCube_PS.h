static const char* skycube_ps_dx = R"(

TextureCube		g_skyTexture;
SamplerState	g_skySampler;

float3			g_cameraPosition;
float4x4		g_cameraProjInvMat;

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

float4 main( const PS_Input Input ) : SV_Target
{
	float2 uv = Input.UV;

	float3 focusedPosition = mul(g_cameraProjInvMat, Input.Position.xyz).xyz;
	float3 viewDir = normalize(focusedPosition-g_cameraPosition);
	float4 color = g_skyTexture.SampleLevel(g_skySampler, viewDir, 0.0);

	return color;
}

)";
