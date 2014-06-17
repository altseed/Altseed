static const char* lightbloom_ps_gl = R"(

uniform vec4 g_weight;

uniform int g_blurredTexture;
uniform sampler2D g_blurredSampler;
uniform int g_originalTexture;
uniform sampler2D g_originalSampler;

vec4 SampleTexture(sampler2D texture_, int sampler_, vec2 uv_){
	return texture2D(texture_, uv_);
}
uvec2 GetTextureSize(sampler2D texture_, int sampler_){
	return (uvec2)textureSize(texture_, 0);
}
vec4 GetOriginalColor(vec2 uv)
{
	return SampleTexture(g_originalTexture, g_originalSampler, , uv);
}

vec4 GetColor(vec2 uv)
{
	return SampleTexture(g_blurredTexture, g_blurredSampler, , uv);
}

vec4 CalcBlurredColor(vec2 uv)
{
	vec2 size = GetTextureSize(g_texture, g_sampler);
#if BLUR_X
	vec2 shift_p = vec2(0.500000 / size.x, 0.500000 / size.y);
	vec2 shift_m = vec2(-0.500000 / size.x, 0.500000 / size.y);
	vec2 adder = vec2(2.00000 / size.x, 0.000000);
#endif
#if BLUR_Y
	vec2 shift_p = vec2(0.500000 / size.x, 0.500000 / size.y);
	vec2 shift_m = vec2(0.500000 / size.x, -0.500000 / size.y);
	vec2 adder = vec2(0.000000, 2.00000 / size.y);
#endif
	vec4 sum = vec4(0.000000, 0.000000, 0.000000, 0.000000);
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

vec4 Main_(vec2 uv)
{
#if BLUR_X
	return CalcBlurredColor(uv);
#endif
#if BLUR_Y
	return max(CalcBlurredColor(uv) - vec4(1.00000, 1.00000, 1.00000, 1.00000), vec4(0.000000, 0.000000, 0.000000, 0.000000)) + GetOriginalColor(uv);
#endif
}



)";