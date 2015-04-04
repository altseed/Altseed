static const char* downsample_ps_dx = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float2			g_offset;

float4 main( const PS_Input Input ) : SV_Target
{
	return g_texture.Sample(g_sampler, Input.UV + g_offset);
}

)";
