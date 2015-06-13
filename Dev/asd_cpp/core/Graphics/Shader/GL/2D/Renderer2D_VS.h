static const char* renderer2d_vs_gl = R"(

in vec3 Pos;
in vec2 UV;
in vec4 Color;

#ifdef HAS_TEXTURE
out vec4 vaTexCoord;
#endif

out vec4 vaColor;

uniform mat4 mat;

void main()
{
	vec4 pos = vec4(Pos.x,Pos.y,0.0,1.0);
	pos = mat * pos;

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