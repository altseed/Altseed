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

float2 Size;

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	Output.Pos.x = Input.Pos.x / Size.x * 2.0 - 1.0;
	Output.Pos.y = -(Input.Pos.y / Size.y * 2.0 - 1.0);
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;

#ifdef HAS_TEXTURE
	Output.UV = Input.UV;
#endif
	Output.Color = Input.Color;
	return Output;
}

)";