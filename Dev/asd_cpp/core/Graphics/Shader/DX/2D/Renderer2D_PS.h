static const char* renderer2d_ps_dx = R"(

#ifdef HAS_TEXTURE
Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );
#endif

struct PS_Input
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float2 UVSub1	: TEXCOORD1;
	float4 Color	: COLOR0;
};


float4 main( const PS_Input Input ) : SV_Target
{
#ifdef HAS_TEXTURE
	float4 Output = g_texture.Sample(g_sampler, Input.UV) * Input.Color;
#else
	float4 Output = Input.Color;
#endif
	if(Output.a == 0.0f) discard;
	return Output;
}

)";