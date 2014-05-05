static const char* paste_ps_dx = R"(


Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );


struct PS_Input
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};


float4 main( const PS_Input Input ) : SV_Target
{
	float2 uv = Input.UV;
	// uv.y = 1.0 - uv.y;

	float4 Output = g_texture.Sample(g_sampler, Input.UV);
	if(Output.a == 0.0f) discard;
	return Output;
}

)";