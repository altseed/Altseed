static const char* terrain_internal_vs_gl = R"(

//<|| モデル共通レジスタ
uniform mat4	matM;
uniform mat4	matC;
uniform mat4	matP;
//||>

//<|| モデル共通頂点入力
in vec3 Position;
in vec3 Normal;
in vec3 Binormal;
in vec2 UV;
in vec2 UVSub;
in vec4 Color;
//||>

out vec4 voutPosition;
out vec4 voutProjPosition;
out vec4 voutColor;
out vec2 voutUV;
out vec2 voutUVSub;
out vec3 voutNormal;
out vec3 voutBinormal;
out vec3 voutTangent;

//<|| モデル共通関数
mat3 convert44to33(mat4 mat)
{
	return mat3(mat[0].xyz, mat[1].xyz, mat[2].xyz);
}
//||>


void main()
{
	mat4 matLocal = matM;
	mat4 matMC = matC * matLocal;
	mat3 matC33 = convert44to33(matC);
	mat3 matMC33 = convert44to33(matMC);

	vec4 cPosition = matMC * vec4( Position.x, Position.y, Position.z, 1.0 );

	vec3 cNormal = matMC33 * Normal;
	//vec3 cNormal =( matMC * vec4(Normal,0.0) ).xyz;
	cNormal = normalize(cNormal);

	vec3 cBinormal = matMC33 * Binormal;
	cBinormal = normalize(cBinormal);

	vec3 cTangent = cross( cBinormal, cNormal );
	cTangent = normalize(cTangent);

	voutPosition = matP * cPosition;
	voutNormal = cNormal;
	voutBinormal = cBinormal;
	voutTangent = cTangent;
	voutUV = UV;
	voutUVSub = UVSub;
	voutColor = Color;
	voutProjPosition = voutPosition;

	gl_Position = voutPosition;
}

)";