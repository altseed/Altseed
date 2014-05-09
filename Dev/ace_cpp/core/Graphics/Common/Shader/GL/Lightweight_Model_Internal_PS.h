static const char* lightweight_model_internal_ps_gl = R"(
#version 330

//<|| モデル共通レジスタ
uniform sampler2D g_colorTexture;
//>||

in vec4 voutPosition;
in vec4 voutColor;
in vec2 voutUV;
in vec3 voutNormal;

out vec4 outOutput0;

void main()
{
	outOutput0 = voutColor;
	if(outOutput0.a == 0.0f) discard;

	outOutput0 = outOutput0 * texture2D(g_colorTexture, voutUV);
	if(outOutput0.a == 0.0f) discard;
}

)";
