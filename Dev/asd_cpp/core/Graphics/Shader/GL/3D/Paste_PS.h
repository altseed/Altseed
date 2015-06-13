static const char* paste_ps_gl = R"(

in vec4 voutPosition;
in vec2 voutUV;

uniform sampler2D g_texture;

out vec4 outOutput0;

void main() 
{
	vec2 uv = voutUV;
	uv.y = 1.0 - uv.y;

	outOutput0 = texture(g_texture, uv);
	if(outOutput0.a == 0.0f) discard;
}

)";