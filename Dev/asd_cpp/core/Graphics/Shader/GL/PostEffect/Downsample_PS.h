static const char* downsample_ps_gl = R"(

uniform sampler2D	g_texture;
uniform vec2		g_offset;

void main()
{
	vec4 output_ = texture(g_texture, inUV + g_offset);

	// 理由は不明だが必要
	output_.a = 1.0;

	outOutput = output_;
}

)";
