static const char* gaussianblur_ps_dx = R"(
Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );
float3          g_weight        : register( c0 );
float4 main( const PS_Input Input ) : SV_Target
{
	uint width, height;
	g_texture.GetDimensions(width, height);

#ifdef BLUR_X
	float2 accum = float2(1.0 / width, 0.0);
	float2 half_ = float2(0.5 / width, 0.0);
	float2 adder = float2(2.0 / width, 0.0);
#endif

#ifdef BLUR_Y
	float2 accum = float2(0.0, 1.0 / height);
	float2 half_ = float2(0.0, 0.5 / height);
	float2 adder = float2(0.0, 2.0 / height);
#endif

	float4 output_ = (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	                 g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	                 g_weight.x;
	if(output_.a == 0.0f) discard;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.y;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.z;

	output_.a = 1.0;

	return output_;
}
)";
