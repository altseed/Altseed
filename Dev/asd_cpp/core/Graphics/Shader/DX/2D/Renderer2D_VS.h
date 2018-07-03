static const char* renderer2d_vs_dx = R"(

struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
	float2 UVSub1	: UV1;
	float4 Color	: COLOR0;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float2 UVSub1	: TEXCOORD1;
	float4 Color	: COLOR0;
};

float4x4 mat;

VS_Output main( const VS_Input Input )
{
	float4 pos = float4(Input.Pos.x,Input.Pos.y,0.0,1.0);
	pos = mul(mat,pos);

	VS_Output Output = (VS_Output)0;
	Output.Pos.x = pos.x;
	Output.Pos.y = pos.y;
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;

	Output.UV = Input.UV;
	Output.UVSub1 = Input.UVSub1;

	Output.Color = Input.Color;
	return Output;
}

)";