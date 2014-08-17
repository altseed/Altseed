static const char* renderer2d_ps_gl = R"(

#ifdef HAS_TEXTURE
in vec4 vaTexCoord;
#endif
in vec4 vaColor;

uniform sampler2D g_texture;

void main() 
{
#ifdef HAS_TEXTURE
	// varying(in) は変更不可(Radeon)

	// gl only
	vec4 vaTexCoord_ = vaTexCoord;
	vaTexCoord_.y = 1.0 - vaTexCoord_.y;

	gl_FragColor = texture2D(g_texture, vaTexCoord_.xy) * vaColor;
#else
	gl_FragColor = vaColor;
#endif
}

)";