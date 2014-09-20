static const char* sprite_renderer3d_ps_gl = R"(

#ifdef HAS_TEXTURE
in vec4 vaTexCoord;
#endif
in vec4 vaColor;

out vec4 outColor;

uniform sampler2D g_texture;

void main() 
{
#ifdef HAS_TEXTURE
	// varying(in) は変更不可(Radeon)

	// gl only
	vec4 vaTexCoord_ = vaTexCoord;
	vaTexCoord_.y = 1.0 - vaTexCoord_.y;

	outColor = texture(g_texture, vaTexCoord_.xy) * vaColor;
#else
	outColor = vaColor;
#endif
}

)";