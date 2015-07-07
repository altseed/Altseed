static const char* downsample_ps_gl = R"(

uniform sampler2D	g_texture;
uniform vec2		g_offset;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

void main()
{
	vec4 output_ = texture(g_texture, inUV + g_offset);

	// 理由は不明だが必要
	output_.a = 1.0;

	outOutput = output_;
}

)";
