static const char* renderer2d_vs_gl = R"(

attribute vec3 Pos;
attribute vec2 UV;
attribute vec4 Color;

#ifdef HAS_TEXTURE
varying vec4 vaTexCoord;
#endif

varying vec4 vaColor;

uniform vec4 Size;

void main()
{
	gl_Position.x = Pos.x / Size.x * 2.0 - 1.0;
	gl_Position.y = -(Pos.y / Size.y * 2.0 - 1.0);
	gl_Position.z = 0.5;
	gl_Position.w = 1.0;

#ifdef HAS_TEXTURE
	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
#endif
	vaColor = Color;
}

)";