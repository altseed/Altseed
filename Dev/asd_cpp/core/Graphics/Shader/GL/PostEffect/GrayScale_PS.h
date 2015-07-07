static const char* grayscale_ps_gl = R"(

uniform sampler2D g_texture;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

void main()
{
	vec4 outputedColor = texture(g_texture, inUV.xy);
	float y = outputedColor.r * 0.298912 + outputedColor.g * 0.586611 + outputedColor.b * 0.114478;
	//outputedColor.xyz = y; (特定環境でコンパイル不可)
	outputedColor.rgb = vec3(y,y,y);
	outOutput = outputedColor; 
}

)";