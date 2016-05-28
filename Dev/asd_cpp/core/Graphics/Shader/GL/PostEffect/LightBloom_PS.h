static const char* lightbloom_ps_gl = R"(

uniform vec4 g_weight1;
uniform vec4 g_weight2;
uniform float g_threshold;
uniform float g_exposure;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

#ifdef BLUR_X
uniform sampler2D g_blurredTexture;
uniform int g_blurredSampler;
#endif

#ifdef BLUR_Y
uniform sampler2D g_blurredTexture;
uniform int g_blurredSampler;
#endif

#ifdef SUM
uniform sampler2D g_originalTexture;
uniform int g_originalSampler;

uniform sampler2D g_blurred0Texture;
uniform int g_blurred0Sampler;

uniform sampler2D g_blurred1Texture;
uniform int g_blurred1Sampler;

uniform sampler2D g_blurred2Texture;
uniform int g_blurred2Sampler;

#endif



vec4 SampleTexture(sampler2D texture_, int sampler_, vec2 uv_){
	return texture(texture_, uv_);
}
uvec2 GetTextureSize(sampler2D texture_, int sampler_){
	return uvec2(textureSize(texture_, 0));
}
float GetLuminance(vec3 color)
{
	return color.x * 0.300000 + color.y * 0.590000 + color.z * 0.110000;
}

vec4 GetColor(vec2 uv)
{
#ifdef BLUR_X
	vec4 color = SampleTexture(g_blurredTexture, g_blurredSampler, uv) * g_exposure;
	color.xyz = min(color.xyz, vec3(255.000, 255.000, 255.000));
#ifdef LUM
	float lum = GetLuminance(color.xyz);
	float bloomedLum = lum - g_threshold;
	float bloomedPower = min(max(bloomedLum / 2.00000, 0.000000), 1.00000);
	return color * bloomedPower;
#else
	vec3 bloomedLum = color.xyz - vec3(g_threshold, g_threshold, g_threshold);
	bloomedLum.xyz = max(bloomedLum.xyz, vec3(0.000000, 0.000000, 0.000000));
	color.xyz = bloomedLum;
	return color;
#endif
#endif
#ifdef BLUR_Y
	return SampleTexture(g_blurredTexture, g_blurredSampler, uv);
#endif
#ifdef SUM
	return vec4(0.000000, 0.000000, 0.000000, 0.000000);
#endif
}

vec4 CalcBlurredColor(vec2 uv)
{
#ifdef SUM
	return vec4(0.000000, 0.000000, 0.000000, 0.000000);
#endif
#ifdef BLUR_X
	vec2 size = GetTextureSize(g_blurredTexture, g_blurredSampler);
#endif
#ifdef BLUR_Y
	vec2 size = GetTextureSize(g_blurredTexture, g_blurredSampler);
#endif
#ifdef SUM
	vec2 size = vec2(0.000000, 0.000000);
	vec2 shift_p = vec2(0.500000 / size.x, -0.500000 / size.y);
	vec2 shift_m = vec2(-0.50000 / size.x, -0.500000 / size.y);
	vec2 adder = vec2(2.00000 / size.x, 0.000000);
#endif
#ifdef BLUR_X
	vec2 shift_p = vec2(0.500000 / size.x, -0.500000 / size.y);
	vec2 shift_m = vec2(-0.50000 / size.x, -0.500000 / size.y);
	vec2 adder = vec2(2.00000 / size.x, 0.000000);
#endif
#ifdef BLUR_Y
	vec2 shift_p = vec2(-0.500000 / size.x, 0.500000 / size.y);
	vec2 shift_m = vec2(-0.500000 / size.x, -0.50000 / size.y);
	vec2 adder = vec2(0.000000, 2.00000 / size.y);
#endif
	vec4 sum = vec4(0.000000, 0.000000, 0.000000, 0.000000);
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
	sum.a = 1.00000;
	return sum;
}

vec4 Main_(vec2 uv)
{
#ifdef BLUR_X
	return CalcBlurredColor(uv);
#endif
#ifdef BLUR_Y
	return CalcBlurredColor(uv);
#endif
#ifdef SUM
	return SampleTexture(g_originalTexture, g_originalSampler, uv) + SampleTexture(g_blurred0Texture, g_blurred0Sampler, uv) * 0.50000 + SampleTexture(g_blurred1Texture, g_blurred1Sampler, uv) * 0.300000 + SampleTexture(g_blurred2Texture, g_blurred2Sampler, uv) * 0.20000;
#endif
}


void main()
{
	outOutput = Main_(inUV);
	outOutput.a = 1.0;
}


)";