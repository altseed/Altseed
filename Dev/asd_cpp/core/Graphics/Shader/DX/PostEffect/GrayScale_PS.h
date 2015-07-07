static const char* grayscale_ps_dx = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

struct PS_Input
{
	float4 SV_Position	: SV_POSITION;
	float4 Position		: POSITION;
	float2 UV			: UV;
	float4 Color		: COLOR;
};

float4 main( const PS_Input Input ) : SV_Target
{
	float4 outputedColor = g_texture.Sample(g_sampler, Input.UV);
	if(outputedColor.a == 0.0f) discard;
	float y = outputedColor.r * 0.298912f + outputedColor.g * 0.586611f + outputedColor.b * 0.114478f;
	outputedColor.rgb = y;
	return outputedColor;
}

)";
