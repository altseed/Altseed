static const char* lightbloom_ps_dx = R"(

float4 g_weight;
Texture2D g_blurredTexture				: register( t0 );
SamplerState g_blurredSampler			: register( s0 );
Texture2D g_originalTexture				: register( t1 );
SamplerState g_originalSampler			: register( s1 );

float4 SampleTexture(Texture2D texture_, SamplerState sampler_, float2 uv_: TEXCOORD): COLOR {
	return texture_.Sample(sampler_, uv_);
}
uint2 GetTextureSize(Texture2D texture_, SamplerState sampler_){
	uint width, height;
	texture_.GetDimension(width, height);
	return uint2(width, height);
}
float4 GetOriginalColor(float2 uv)
{
	return SampleTexture(g_originalTexture, g_originalSampler, , uv);
}

float4 GetColor(float2 uv)
{
	return SampleTexture(g_blurredTexture, g_blurredSampler, , uv);
}

float4 CalcBlurredColor(float2 uv)
{
	float2 size = GetTextureSize(g_texture, g_sampler);
#if BLUR_X
	float2 shift_p = float2(0.500000 / size.x, 0.500000 / size.y);
	float2 shift_m = float2(-0.500000 / size.x, 0.500000 / size.y);
	float2 adder = float2(2.00000f / size.x, 0.000000);
#endif
#if BLUR_Y
	float2 shift_p = float2(0.500000 / size.x, 0.500000 / size.y);
	float2 shift_m = float2(0.500000 / size.x, -0.500000 / size.y);
	float2 adder = float2(0.000000, 2.00000 / size.y);
#endif
	float4 sum = float4(0.000000, 0.000000, 0.000000, 0.000000);
	sum += g_weight.x * GetColor(uv + shift_p + adder * 0.000000);
	sum += g_weight.x * GetColor(uv + shift_m - adder * 0.000000);
	sum += g_weight.y * GetColor(uv + shift_p + adder * 1.00000);
	sum += g_weight.y * GetColor(uv + shift_m - adder * 1.00000);
	sum += g_weight.z * GetColor(uv + shift_p + adder * 2.00000);
	sum += g_weight.z * GetColor(uv + shift_m - adder * 2.00000);
	sum += g_weight.w * GetColor(uv + shift_p + adder * 3.00000);
	sum += g_weight.w * GetColor(uv + shift_m - adder * 3.00000);
	return sum;
}

float4 Main_(float2 uv)
{
#if BLUR_X
	return CalcBlurredColor(uv);
#endif
#if BLUR_Y
	return max(CalcBlurredColor(uv) - float4(1.00000, 1.00000, 1.00000, 1.00000), float4(0.000000, 0.000000, 0.000000, 0.000000)) + GetOriginalColor(uv);
#endif
}



)";