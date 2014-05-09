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
	return mat3(tangent, binormal, normal) * n;
}

float CalculateDepth(vec4 position)
{
	return position.z / position.w;
}
//>||


void main()
{
	vec4 diffuseColor = voutColor;
	if(diffuseColor.a == 0.0f) discard;

	diffuseColor = diffuseColor * texture2D(g_colorTexture, voutUV);
	if(diffuseColor.a == 0.0f) discard;	

#ifdef EXPORT_DEPTH
	Depth.x = CalculateDepth( voutPosition );
	Depth = vec4( Depth.x, Depth.y, Depth.z, Depth.w );

#else

	DiffuseColor = diffuseColor;

	NormalDepth.xyz = CalculateNormal( voutNormal, voutTangent, voutBinormal, texture2D(g_normalTexture, voutUV).xyz );
	NormalDepth.w = CalculateDepth( voutPosition );

	SpecularColor_Smoothness.xyz = texture2D(g_specularTexture, voutUV).xyz;
	SpecularColor_Smoothness.w = 0.5;

	AO_MatID.x = 1.0;
	AO_MatID.y = 0;
	AO_MatID.z = 0;
	AO_MatID.w = 0;

#endif

}

)";