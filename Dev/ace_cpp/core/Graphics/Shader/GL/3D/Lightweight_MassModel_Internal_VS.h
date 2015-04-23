static const char* lightweight_mass_model_internal_vs_gl = R"(

//<|| モデル共通レジスタ
uniform mat4	matM[32];
uniform mat4	matC;
uniform mat4	matP;
//||>

uniform vec4	animationParam0[32];
uniform vec4	animationParam1[32];

uniform vec3		directionalLightDirection;
uniform vec3		directionalLightColor;
uniform vec3		skyLightColor;
uniform vec3		groundLightColor;

//<|| モデル共通頂点入力
in vec3 Position;
in vec3 Normal;
in vec3 Binormal;
in vec2 UV;
in vec2 UVSub;
in vec4 Color;
in vec4 BoneWeights;
in vec4 BoneIndexes;
//in vec4 BoneIndexesOriginal;
//||>


out vec4 voutPosition;
out vec4 voutColor;
out vec2 voutUV;
out vec2 voutUVSub;
out vec3 voutNormal;


//<|| ALSL

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
//||>

uniform sampler2D g_animationTexture;

mat4 getMatrix(int animationIndex, int boneIndex, float time)
{
	vec2 size = vec2(textureSize(g_animationTexture, 0));
	float width = size.x;
	float height = size.y;

	float x = time / float(width);
	int yind = animationIndex * 32 * 4 + boneIndex * 4;
	float y0 = (float(yind) + 0.0 + 0.5) / float(height);
	float y1 = (float(yind) + 1.0 + 0.5) / float(height);
	float y2 = (float(yind) + 2.0 + 0.5) / float(height);
	float y3 = (float(yind) + 3.0 + 0.5) / float(height);

	vec4 y0v = textureLod(g_animationTexture, vec2(x,y0), 0);
	vec4 y1v = textureLod(g_animationTexture, vec2(x,y1), 0);
	vec4 y2v = textureLod(g_animationTexture, vec2(x,y2), 0);
	vec4 y3v = textureLod(g_animationTexture, vec2(x,y3), 0);

	return transpose(mat4( y0v, y1v, y2v, y3v ));
}

mat4 calcMatrix(int animationIndex, float time, vec4 weights, vec4 indexes)
{
	/*
	return getMatrix(int(animationIndex), int(indexes.x), time) * weights.x +
	getMatrix(int(animationIndex), int(indexes.y), time) * weights.y +
	getMatrix(int(animationIndex), int(indexes.z), time) * weights.z +
	getMatrix(int(animationIndex), int(indexes.w), time) * weights.w;
	*/
	return getMatrix(int(animationIndex), int(indexes.x), time) * weights.x +
	getMatrix(int(animationIndex), int(indexes.y), time) * weights.y;
}

mat3 convert44to33(mat4 mat)
{
	return mat3(mat[0].xyz, mat[1].xyz, mat[2].xyz);
}

void main()
{
	int instanceId = gl_InstanceID; 

#ifdef ANIMATION_IS_ENABLED
	float animIndex0 = animationParam0[instanceId].x;
	float animIndex1 = animationParam0[instanceId].y;
	float animTime0 = animationParam0[instanceId].z;
	float animTime1 = animationParam0[instanceId].w;
	float animWeight = animationParam1[instanceId].x;
	mat4 matModel = matM[instanceId];

	mat4 matLocal0 = calcMatrix(int(animIndex0), animTime0, BoneWeights, BoneIndexes);
	mat4 matLocal1 = calcMatrix(int(animIndex1), animTime1, BoneWeights, BoneIndexes);
	mat4 matLocal = matLocal0 + (matLocal1 - matLocal0) * animWeight;
	matLocal = matModel * matLocal;
#else
	mat4 matLocal = matM[instanceId];
#endif

	mat4 matMC = matC * matLocal;
	mat3 matC33 = convert44to33(matC);
	mat3 matMC33 = convert44to33(matMC);

	vec4 cPosition = matMC * vec4( Position.x, Position.y, Position.z, 1.0 );

	vec3 cNormal = matMC33 * Normal;
	cNormal = normalize(cNormal);

	vec3 cDirectionalLightDirection = (matC33 * directionalLightDirection).xyz;
	
	vec3 upDir = matC33 * vec3(0.0,1.0,0.0);

	voutPosition = matP * cPosition;
	voutNormal = cNormal.xyz;
	voutUV = UV;
	voutUVSub = UVSub;

	voutColor = Color * calcLightColor(upDir,cNormal,cDirectionalLightDirection);

	gl_Position = voutPosition;
}

)";