static const char* screen_vs_gl = R"(

in vec3 Position;
in vec2 UV;

out vec4 voutPosition;
out vec2 voutUV;

void main()
{
	voutPosition.x = Position.x;
	voutPosition.y = Position.y;
	voutPosition.z = 0.5;
	voutPosition.w = 1.0;

	voutUV = UV;

	gl_Position = voutPosition;
}

)";