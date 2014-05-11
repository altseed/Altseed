static const char* blur_ps_dx = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );
float4          g_weight        : register( c0 );


struct PS_Input
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

half2 GetTextureSize(Texture2D texture_)
{
	uint width, height;
	texture_.GetDimensions(width, height);
	return half2(width,height);
}

float4 main( const PS_Input Input ) : SV_Target
{
	half2 textureSize = GetTextureSize(g_texture);

#ifdef BLUR_X
	float2 accum = float2(1.0 / textureSize.x, 0.0);
	float2 half_ = float2(0.5 / textureSize.x, 0.0);
	float2 adder = float2(2.0 / textureSize.x, 0.0);
#else
	float2 accum = float2(0.0, 1.0 / textureSize.y);
	float2 half_ = float2(0.0, 0.5 / textureSize.y);
	float2 adder = float2(0.0, 2.0 / textureSize.y);
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
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.w;

	return output_;
}

)";