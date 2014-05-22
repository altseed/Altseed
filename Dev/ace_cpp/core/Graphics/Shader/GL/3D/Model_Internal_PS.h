static const char* model_internal_ps_gl = R"(
#version 330

//<|| モデル共通レジスタ
uniform sampler2D g_colorTexture;
//>||

uniform sampler2D g_normalTexture;
uniform sampler2D g_specularTexture;

in vec4 voutPosition;
in vec4 voutColor;
in vec2 voutUV;
in vec3 voutNormal;
in vec3 voutBinormal;
in vec3 voutTangent;
in	float voutDepth;

#ifdef EXPORT_DEPTH
out vec4 Depth;
#else
layout(location = 0) out vec4 DiffuseColor;
layout(location = 1) out vec4 SpecularColor_Smoothness;
layout(location = 2) out vec4 NormalDepth;
layout(location = 3) out vec4 AO_MatID;
#endif

//<|| ALSL
vec3 CalculateNormal(vec3 normal, vec3 tangent, vec3 binormal, vec3 normalMap)
{
	vec3 n = (normalMap - 0.500000) * 2.00000;
	return tangent * n.x + binormal * n.y + normal * n.z;
}

//>||


void main()
{
	vec2 voutUV_ = voutUV;
	voutUV_.y = 1.0 - voutUV_.y;

	vec4 diffuseColor = voutColor;
	if(diffuseColor.a == 0.0f) discard;

	diffuseColor = diffuseColor * texture2D(g_colorTexture, voutUV_);
	if(diffuseColor.a == 0.0f) discard;	

#ifdef EXPORT_DEPTH
	// VSM用二乗バッファ含む
	Depth.x = voutDepth;
	Depth.y = voutDepth * voutDepth;
	Depth.z = 0.0;
	Depth.w = voutDepth;
#else

	DiffuseColor = diffuseColor;

	NormalDepth.xyz = CalculateNormal( voutNormal, voutTangent, voutBinormal, texture2D(g_normalTexture, voutUV_).xyz );
	//NormalDepth.xyz = texture2D(g_normalTexture, voutUV_).xyz;
	NormalDepth.w = voutDepth;
	//NormalDepth.xyz = voutNormal;

	//NormalDepth.x = 1.0;
	//NormalDepth.y = 1.0;
	//NormalDepth.z = 1.0;
	//NormalDepth.w = 1.0;
	//NormalDepth = diffuseColor;

	SpecularColor_Smoothness.xyz = texture2D(g_specularTexture, voutUV_).xyz;
	SpecularColor_Smoothness.w = 0.5;

	AO_MatID.x = 1.0;
	AO_MatID.y = 0;
	AO_MatID.z = 0;
	AO_MatID.w = 0;

#endif

}

)";