static const char* sepia_ps_gl = R"(
uniform sampler2D g_texture;

void main()
{
	vec4 outputedColor = texture(g_texture, inUV.xy);
	float y = outputedColor.r * 0.298912 + outputedColor.g * 0.586611 + outputedColor.b * 0.114478;
	//outputedColor.rgb = y; (特定環境でコンパイル不可)
	outputedColor.rgb = vec3(y,y,y);
	outputedColor.r *= 1.332249;
	outputedColor.r = outputedColor.r > 1.0? 1.0: outputedColor.r;
	outputedColor.g *= 0.921369;
	outputedColor.b *= 0.535390;
	outOutput = outputedColor; 
}

)";