static const char* lightweight_model_internal_ps_gl = R"(

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
	vec2 voutUV_ = voutUV;
	voutUV_.y = 1.0 - voutUV_.y;

	outOutput0 = voutColor;
	if(outOutput0.a == 0.0f) discard;

	outOutput0 = outOutput0 * texture(g_colorTexture, voutUV_);
	if(outOutput0.a == 0.0f) discard;
}

)";
