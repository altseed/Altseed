static const char* blur_ps_gl = R"(
#version 130

in vec4 voutPosition;
in vec2 voutUV;

uniform sampler2D g_texture;
uniform vec4      g_weight;

out vec4 outOutput0;

vec2 GetTextureSize(sampler2D texture_)
{
	return textureSize(texture_, 0);
}

void main()
{
	vec2 textureSize = GetTextureSize(g_texture);

#ifdef BLUR_X
	vec2 accum = vec2(1.0 / textureSize.x, 0.0);
	vec2 half_ = vec2(0.5 / textureSize.x, 0.0);
	vec2 adder = vec2(2.0 / textureSize.x, 0.0);
#else
	vec2 accum = vec2(0.0, 1.0 / textureSize.y);
	vec2 half_ = vec2(0.0, 0.5 / textureSize.y);
	vec2 adder = vec2(0.0, 2.0 / textureSize.y);
#endif

	vec4 output_ = (texture2D(g_texture, voutUV.xy + half_ + accum)  +
	               texture2D(g_texture, voutUV.xy + half_ - accum)) *
	               g_weight.x;
	accum += adder;
	output_ += (texture2D(g_texture, voutUV.xy + half_ + accum)  +
	          texture2D(g_texture, voutUV.xy + half_ - accum)) *
	          g_weight.y;
	accum += adder;
	output_ += (texture2D(g_texture, voutUV.xy + half_ + accum)  +
	          texture2D(g_texture, voutUV.xy + half_ - accum)) *
	          g_weight.z;
	accum += adder;
	output_ += (texture2D(g_texture, voutUV.xy + half_ + accum)  +
	          texture2D(g_texture, voutUV.xy + half_ - accum)) *
	          g_weight.w;

	outOutput0 = output_; 
}

)";