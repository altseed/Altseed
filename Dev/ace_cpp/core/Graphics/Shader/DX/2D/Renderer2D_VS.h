static const char* renderer2d_vs_dx = R"(

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

float4 area;

VS_Output main( const VS_Input Input )
{
	float2 pos = float2(Input.Pos.x,Input.Pos.y);
	pos.x = (pos.x - area.x) / area.z * 2.0;
	pos.y = (pos.y - area.y) / area.w * 2.0;
	pos.x = pos.x - 1.0;
	pos.y = -pos.y + 1.0;

	VS_Output Output = (VS_Output)0;
	Output.Pos.x = pos.x;
	Output.Pos.y = pos.y;
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;

#ifdef HAS_TEXTURE
	Output.UV = Input.UV;
#endif
	Output.Color = Input.Color;
	return Output;
}

)";