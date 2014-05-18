static const char* layerrenderer_vs_dx = R"(
struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
	float4 Color	: COLOR0;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR0;
};

float2 Size;
float4 LayerPosU;
float4 LayerPosL;

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	float2 ul = LayerPosU.xy / Size.xy;
	float2 ur = LayerPosU.zw / Size.xy;
	float2 lr = LayerPosL.xy / Size.xy;
	float2 ll = LayerPosL.zw / Size.xy;

	float2 pos = float2( Input.Pos.x, Input.Pos.y );
  
	float2 u = (ur - ul) * pos.x + ul;
	float2 l = (lr - ll) * pos.x + ll;
	float2 p = (l - u) * pos.y + u;

	Output.Pos.x = p.x * 2.0 - 1.0;
	Output.Pos.y = -(p.y * 2.0 - 1.0);
 
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;

	Output.UV = Input.UV;
	Output.Color = Input.Color;
	return Output;
}

)";