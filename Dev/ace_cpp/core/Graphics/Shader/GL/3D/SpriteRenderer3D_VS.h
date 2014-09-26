static const char* sprite_renderer3d_vs_gl = R"(

in vec3 Pos;
in vec2 UV;
in vec4 Color;

#ifdef HAS_TEXTURE
out vec4 vaTexCoord;
#endif

out vec4 vaColor;

uniform mat4	cameraMatrix;
uniform mat4	projectionMatrix;

void main()
{
	mat4 cameraProjMat = projectionMatrix * cameraMatrix;
	vec4 pos = cameraProjMat * vec4(Pos,1.0);

	gl_Position = pos;

#ifdef HAS_TEXTURE
	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
#endif
	vaColor = Color;
}

)";