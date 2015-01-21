static const char* lightbloom_ps_dx = R"(

float4 g_weight1;
float4 g_weight2;

float g_threshold;
float g_power;

Texture2D g_blurredTexture				: register( t0 );
SamplerState g_blurredSampler			: register( s0 );
Texture2D g_originalTexture				: register( t1 );
SamplerState g_originalSampler			: register( s1 );


float4 SampleTexture(Texture2D texture_, SamplerState sampler_, float2 uv_) {
	return texture_.Sample(sampler_, uv_);
}
uint2 GetTextureSize(Texture2D texture_, SamplerState sampler_){
	uint width, height;
	texture_.GetDimensions(width, height);
	return uint2(width, height);
}
float4 GetOriginalColor(float2 uv)
{
	return SampleTexture(g_originalTexture, g_originalSampler, uv);
}

float4 GetColor(float2 uv)
{
#if BLUR_X
	return max(SampleTexture(g_blurredTexture, g_blurredSampler, uv)-float4(g_threshold,g_threshold,g_threshold,0.0), float4(0.0,0.0,0.0,0.0));
#endif
	return SampleTexture(g_blurredTexture, g_blurredSampler, uv);
}

float4 CalcBlurredColor(float2 uv)
{
	float2 size = GetTextureSize(g_blurredTexture, g_blurredSampler);
#if COPY
	float2 shift_p = float2(0.000000, 0.000000);
	float2 shift_m = float2(0.000000, 0.000000);
	float2 adder = float2(0.000000, 0.000000);
#endif
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
	sum += g_weight1.x * GetColor(uv + shift_p + adder * 0.000000);
	sum += g_weight1.x * GetColor(uv + shift_m - adder * 0.000000);
	sum += g_weight1.y * GetColor(uv + shift_p + adder * 1.00000);
	sum += g_weight1.y * GetColor(uv + shift_m - adder * 1.00000);
	sum += g_weight1.z * GetColor(uv + shift_p + adder * 2.00000);
	sum += g_weight1.z * GetColor(uv + shift_m - adder * 2.00000);
	sum += g_weight1.w * GetColor(uv + shift_p + adder * 3.00000);
	sum += g_weight1.w * GetColor(uv + shift_m - adder * 3.00000);
	sum += g_weight2.x * GetColor(uv + shift_p + adder * 4.000000);
	sum += g_weight2.x * GetColor(uv + shift_m - adder * 4.000000);
	sum += g_weight2.y * GetColor(uv + shift_p + adder * 5.00000);
	sum += g_weight2.y * GetColor(uv + shift_m - adder * 5.00000);
	sum += g_weight2.z * GetColor(uv + shift_p + adder * 6.00000);
	sum += g_weight2.z * GetColor(uv + shift_m - adder * 6.00000);
	sum += g_weight2.w * GetColor(uv + shift_p + adder * 7.00000);
	sum += g_weight2.w * GetColor(uv + shift_m - adder * 7.00000);

	sum.a = 1.0;

	return sum;
}

float4 Main_(float2 uv)
{
#if COPY
	return GetOriginalColor(uv);
#endif
#if BLUR_X
	return CalcBlurredColor(uv);
#endif
#if BLUR_Y
	return max(CalcBlurredColor(uv) * g_power, float4(0.000000, 0.000000, 0.000000, 0.000000)) + GetOriginalColor(uv);
#endif
}

float4 main( const PS_Input Input ) : SV_Target
{
	float4 o = Main_(Input.UV);
	o.a = 1.0;
	return o;
}


)";