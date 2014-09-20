static const char* sprite_renderer3d_vs_gl = R"(

in vec3 Pos;
in vec2 UV;
in vec4 Color;

#ifdef HAS_TEXTURE
out vec4 vaTexCoord;
#endif

out vec4 vaColor;

uniform vec4 area;

void main()
{
	vec2 pos = vec2(Pos.x,Pos.y);
	pos.x = (pos.x - area.x) / area.z * 2.0;
	pos.y = (pos.y - area.y) / area.w * 2.0;
	pos.x = pos.x - 1.0;
	pos.y = -pos.y + 1.0;

	gl_Position.x = pos.x;
	gl_Position.y = pos.y;
	gl_Position.z = 0.5;
	gl_Position.w = 1.0;

#ifdef HAS_TEXTURE
	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
#endif
	vaColor = Color;
}

)";