static const char* sprite_renderer3d_vs_dx = R"(

struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
	float4 Color	: COLOR0;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;

#ifdef HAS_TEXTURE
	float2 UV		: TEXCOORD0;
#endif
	float4 Color	: COLOR0;
};

float4x4	cameraMatrix;
float4x4	projectionMatrix;

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	float4x4 cameraProjMat =mul(projectionMatrix, cameraMatrix);
	float4 pos = mul(cameraProjMat, float4(Input.Pos,1.0));

	Output.Pos = pos;

#ifdef HAS_TEXTURE
	Output.UV = Input.UV;
#endif
	Output.Color = Input.Color;
	return Output;
}

)";