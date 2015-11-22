static const char* screen_vs_dx = R"(

struct VS_Input
{
	float3 Position		: POSITION0;
	float2 UV			: UV0;
};

struct VS_Output
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	Output.Position.x = Input.Position.x;
	Output.Position.y = Input.Position.y;
	Output.Position.z = Input.Position.z;
	Output.Position.w = 1.0;

	Output.UV = Input.UV;

	Output.SV_Position = Output.Position;

	return Output;
}

)";