static const char* lightweight_terrain_internal_vs_gl = R"(

//<|| モデル共通レジスタ
uniform mat4	matM;
uniform mat4	matC;
uniform mat4	matP;
//||>

#ifndef BLACK
uniform vec3		directionalLightDirection;
uniform vec3		directionalLightColor;
uniform vec3		skyLightColor;
uniform vec3		groundLightColor;
#endif

//<|| モデル共通頂点入力
in vec3 Position;
in vec3 Normal;
in vec3 Binormal;
in vec2 UV;
in vec2 UVSub;
in vec4 Color;
//||>


out vec4 voutPosition;
out vec4 voutColor;
out vec2 voutUV;
out vec2 voutUVSub;
out vec3 voutNormal;


//<|| ALSL
mat3 convert44to33(mat4 mat)
{
	return mat3(mat[0].xyz, mat[1].xyz, mat[2].xyz);
	
}

#ifndef BLACK
vec4 calcLightColor(vec3 upDir, vec3 normal, vec3 lightDir)
{
	vec4 color = vec4(0.000000, 0.000000, 0.000000, 1.00000);
	float NoL = dot(normal, lightDir);
	float NoU = dot(normal, upDir);
	color.xyz = directionalLightColor * max(NoL, 0.000000);
	color.xyz = color.xyz + skyLightColor * max(NoU+1,0.0) / 2.0;
	color.xyz = color.xyz + groundLightColor * max(-NoU+1,0.0) / 2.0;
	return color;
	
}
#endif
//||>

void main()
{
	mat4 matLocal = matM;
	mat4 matMC = matC * matLocal;
	mat3 matC33 = convert44to33(matC);
	mat3 matMC33 = convert44to33(matMC);

	vec4 cPosition = matMC * vec4( Position.x, Position.y, Position.z, 1.0 );

	vec3 cNormal = matMC33 * Normal;
	cNormal = normalize(cNormal);

#ifndef BLACK
	vec3 cDirectionalLightDirection = (matC33 * directionalLightDirection).xyz;
#endif

	vec3 upDir = matC33 * vec3(0.0,1.0,0.0);

	voutPosition = matP * cPosition;
	voutNormal = cNormal.xyz;
	voutUV = UV;
	voutUVSub = UVSub;

#ifdef BLACK
	voutColor = Color;
#else
	voutColor = Color * calcLightColor(upDir,cNormal,cDirectionalLightDirection);
#endif

	gl_Position = voutPosition;
}

)";