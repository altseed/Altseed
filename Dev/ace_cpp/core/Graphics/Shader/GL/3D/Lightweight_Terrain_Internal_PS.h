static const char* lightweight_terrain_internal_ps_gl = R"(

//<|| モデル共通レジスタ
uniform sampler2D g_colorTexture;
//>||

uniform sampler2D g_densityTexture;

in vec4 voutPosition;
in vec4 voutColor;
in vec2 voutUV;
in vec2 voutUVSub;
in vec3 voutNormal;

out vec4 outOutput0;

void main()
{
	#ifndef BLACK
	outOutput0 = vec4(0.0,0.0,0.0,1.0);
	return;
	#endif

	vec2 voutUV_ = voutUV;
	voutUV_.y = 1.0 - voutUV_.y;

	vec2 voutUVSub_ = voutUVSub;
	voutUVSub_.y = 1.0 - voutUVSub_.y;

	outOutput0 = voutColor;
	if(outOutput0.a == 0.0f) discard;

	outOutput0 = outOutput0 * texture(g_colorTexture, voutUV_);
	if(outOutput0.a == 0.0f) discard;

	float density = texture(g_densityTexture, voutUVSub_).x;
	outOutput0 = outOutput0 * density;
}

)";
