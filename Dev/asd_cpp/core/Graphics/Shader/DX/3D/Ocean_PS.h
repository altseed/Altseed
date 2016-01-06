static const char* ocean_ps_dx = R"(

Texture2D		g_reflectionTexture		: register( t0 );
SamplerState	g_reflectionSampler		: register( s0 );

Texture2D		g_refractionTexture		: register( t1 );
SamplerState	g_refractionSampler		: register( s1 );

Texture2D		g_normalTexture		: register( t2 );
SamplerState	g_normalSampler		: register( s2 );

Texture2D		g_gbuffer2Texture;
SamplerState	g_gbuffer2Sampler;

Texture2D		g_maskTexture;
SamplerState	g_maskSampler;


float3 reconstructInfo1;
float4 reconstructInfo2;

float3			g_cameraPosition;
float3			g_oceanColor;
float			g_density;

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 ProjPosition		: PROJPOSITION0;
	half2 UV				: UV0;
	half2 UVSub				: UVSub0;
	half3 Normal			: NORMAL0;
	half3 Binormal			: BINORMAL0;
	half3 Tangent			: TANGENT0;
};

half3 CalculateNormal( half3 normal, half3 tangent, half3 binormal, float3 normalMap )
{
	half3 n = (normalMap - 0.5) * 2.0;
	return tangent * n.x + binormal * n.y + normal * n.z;
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
	float4 mask = g_maskTexture.Sample(g_maskSampler, Input.UVSub);
	if(mask.x == 0) discard;

	float2 uv = Input.UV;

	float2 refUV = Input.ProjPosition.xy / Input.ProjPosition.w;
	refUV.x = (1.0-refUV.x) / 2.0;
	refUV.y = (1.0-refUV.y) / 2.0;

	float2 refraUV = Input.ProjPosition.xy / Input.ProjPosition.w;
	refraUV.x = (refraUV.x + 1.0) / 2.0;
	refraUV.y = (1.0-refraUV.y) / 2.0;

	float3 viewerCameraPos = ReconstructPosition(Input.ProjPosition.xy / Input.ProjPosition.w, 0.0);
	float3 objectCameraPos = ReconstructPosition(Input.ProjPosition.xy / Input.ProjPosition.w, ReconstructDepth(GetNormalizedDepth(refraUV)));
	float3 waterCameraPos = Input.Position.xyz / Input.Position.w;

	float len = length(waterCameraPos - objectCameraPos);

	float3 normalMap = g_normalTexture.Sample(g_normalSampler, Input.UV);

	half3 normal = CalculateNormal(Input.Normal, Input.Binormal, Input.Tangent, normalMap);

	float A = 1.0 / 1.33;
    float B = dot(-normalize(Input.Position.xyz), normal);
    float C = sqrt(1.0f - A*A * (1-B*B));
    float Rs = (A*B-C) * (A*B-C) / ((A*B+C) * (A*B+C));
    float Rp = (A*C-B) * (A*C-B) / ((A*C+B) * (A*C+B));
    float alpha = (Rs + Rp) / 2.0f;
	alpha = min(alpha, 1.0);

	// TODO:正しく計算する
	float2 shift = float2( (normalMap.x - 0.5) / 4.0, (normalMap.y - 0.5) / 4.0);

	float4 refColor = g_reflectionTexture.Sample(g_reflectionSampler, refUV + shift);
	float4 refraColor = g_refractionTexture.Sample(g_refractionSampler, refraUV + shift);

	float trans = exp(-len*g_density);

	trans = min(trans, 1.0);

	refraColor.xyz = g_oceanColor * (1.0-trans) + refraColor.xyz * trans;
	refraColor.w = 1.0;
	refColor.w = 1.0;

	//return float4(len,len,len,1.0);
	float4 ret = refColor * alpha + refraColor * (1.0 - alpha);

	ret.xyz = ret.xyz * mask.x;

	return ret;
}

)";
