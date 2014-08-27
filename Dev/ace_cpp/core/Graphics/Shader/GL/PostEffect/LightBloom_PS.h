static const char* lightbloom_ps_gl = R"(

uniform vec4 g_weight;
uniform float g_threshold;
uniform float g_power;

uniform sampler2D g_blurredTexture;
uniform int g_blurredSampler;
uniform sampler2D g_originalTexture;
uniform int g_originalSampler;

vec4 SampleTexture(sampler2D texture_, int sampler_, vec2 uv_){
	return texture(texture_, uv_);
}
uvec2 GetTextureSize(sampler2D texture_, int sampler_){
	return uvec2(textureSize(texture_, 0));
}
vec4 GetOriginalColor(vec2 uv)
{
	return SampleTexture(g_originalTexture, g_originalSampler, uv);
}

vec4 GetColor(vec2 uv)
{
#ifdef BLUR_X
	return max(SampleTexture(g_blurredTexture, g_blurredSampler, uv)-vec4(g_threshold,g_threshold,g_threshold,0.0), vec4(0.0,0.0,0.0,0.0));
#endif
	return SampleTexture( g_blurredTexture, g_blurredSampler, uv );
}

vec4 CalcBlurredColor(vec2 uv)
{
	vec2 size = GetTextureSize(g_blurredTexture, g_blurredSampler);
#ifdef COPY
	vec2 shift_p = vec2(0.000000, 0.000000);
	vec2 shift_m = vec2(0.000000, 0.000000);
	vec2 adder = vec2(0.000000, 0.000000);
#endif
#ifdef BLUR_X
	vec2 shift_p = vec2(0.500000 / size.x, 0.500000 / size.y);
	vec2 shift_m = vec2(-0.500000 / size.x, 0.500000 / size.y);
	vec2 adder = vec2(2.00000 / size.x, 0.000000);
#endif
#ifdef BLUR_Y
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

	sum.a = 1.0;

	return sum;
}

vec4 Main_(vec2 uv)
{
#ifdef COPY
	return GetOriginalColor(uv);
#endif
#ifdef BLUR_X
	return CalcBlurredColor(uv);
#endif
#ifdef BLUR_Y
	return max(CalcBlurredColor(uv) * g_power, vec4(0.000000, 0.000000, 0.000000, 0.000000)) + GetOriginalColor(uv);
#endif
}

void main()
{
	outOutput = Main_(inUV);
	outOutput.a = 1.0;
}


)";