static const char* renderer2d_vs_gl = R"(

in vec3 Pos;
in vec2 UV;
in vec2 UVSubA;
in vec4 Color;

out vec4 vaTexCoord;
out vec4 vaTexCoordSubA;

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

	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
	vaTexCoordSubA = vec4(UVSubA.x,UVSubA.y,0.0,0.0);

	vaColor = Color;
}

)";