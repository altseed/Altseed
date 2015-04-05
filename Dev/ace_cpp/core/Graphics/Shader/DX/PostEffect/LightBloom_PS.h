static const char* lightbloom_ps_dx = R"(

float4 g_weight;
float g_threshold;
float g_exposure;

#if BLUR_X
Texture2D g_blurredTexture				: register( t0 );
SamplerState g_blurredSampler			: register( s0 );
#endif

#if BLUR_Y
Texture2D g_blurredTexture				: register( t0 );
SamplerState g_blurredSampler			: register( s0 );
#endif

#if SUM
Texture2D g_originalTexture				: register( t0 );
SamplerState g_originalSampler			: register( s0 );

Texture2D g_blurred0Texture				: register( t1 );
SamplerState g_blurred0Sampler			: register( s1 );

Texture2D g_blurred1Texture				: register( t2 );
SamplerState g_blurred1Sampler			: register( s2 );

Texture2D g_blurred2Texture				: register( t3 );
SamplerState g_blurred2Sampler			: register( s3 );

#endif

float4 SampleTexture(Texture2D texture_, SamplerState sampler_, float2 uv_) {
	return texture_.Sample(sampler_, uv_);
}
uint2 GetTextureSize(Texture2D texture_, SamplerState sampler_){
	uint width, height;
	texture_.GetDimensions(width, height);
	return uint2(width, height);
}

float GetLuminance(float3 color)
{
	return dot(color, float3(0.3,0.59,0.11)).x;
}

float4 GetColor(float2 uv)
{
#if BLUR_X
	// UnrealEngine4参考
	float4  color = SampleTexture(g_blurredTexture, g_blurredSampler, uv) * g_exposure;
	float lum = GetLuminance(color);
	float bloomedLum = lum - g_threshold;
	float bloomedPower = min(max(bloomedLum / 2.0, 0.0),1.0);
	return color * bloomedPower;
#endif

#if BLUR_Y
	return SampleTexture(g_blurredTexture, g_blurredSampler, uv);
#endif

#if SUM
	return float4(0.0,0.0,0.0,0.0);
#endif
}

float4 CalcBlurredColor(float2 uv)
{
#if SUM
	return float4(0.0,0.0,0.0,0.0);
#endif

#if BLUR_X
	float2 size = GetTextureSize(g_blurredTexture, g_blurredSampler);
#endif

#if BLUR_Y
	float2 size = GetTextureSize(g_blurredTexture, g_blurredSampler);
#endif

#if SUM
	float2 size = float2(0.0,0.0);
	float2 shift_p = float2(0.500000 / size.x, 0.500000 / size.y);
	float2 shift_m = float2(0.500000 / size.x, -1.500000 / size.y);
	float2 adder = float2(0.000000, 2.00000 / size.y);
#endif

#if BLUR_X
	float2 shift_p = float2(0.500000 / size.x, 0.500000 / size.y);
	float2 shift_m = float2(-1.500000 / size.x, 0.500000 / size.y);
	float2 adder = float2(2.00000f / size.x, 0.000000);
#endif

#if BLUR_Y
	float2 shift_p = float2(0.500000 / size.x, 0.500000 / size.y);
	float2 shift_m = float2(0.500000 / size.x, -1.500000 / size.y);
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

	sum.a = 1.0;

	return sum;
}

float4 Main_(float2 uv)
{
#if BLUR_X
	return CalcBlurredColor(uv);
#endif
#if BLUR_Y
	return CalcBlurredColor(uv);
#endif
#if SUM
	return SampleTexture(g_originalTexture, g_originalSampler, uv) +
	SampleTexture(g_blurred0Texture, g_blurred0Sampler, uv) * 0.33 +
	SampleTexture(g_blurred1Texture, g_blurred1Sampler, uv) * 0.33 +
	SampleTexture(g_blurred2Texture, g_blurred2Sampler, uv) * 0.33;
#endif
}

float4 main( const PS_Input Input ) : SV_Target
{
	float4 o = Main_(Input.UV);
	o.a = 1.0;
	return o;
}


)";