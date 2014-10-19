static const char* mass_model_internal_vs_dx = R"(

//<|| モデル共通レジスタ
float4x4	matM[32];
float4x4	matC;
float4x4	matP;
float		animationIndex0[32];
float		animationIndex1[32];
float		animationTime0[32];
float		animationTime1[32];
float		animationWeight[32];

//||>

//<|| モデル共通頂点入力
struct VS_Input
{
	float3 Position		: Position0;
	float3 Normal		: Normal0;
	float3 Binormal		: Binormal0;
	float2 UV			: UV0;
	float2 UVSub		: UVSub0;
	float4 Color		: Color0;
	float4 BoneWeights	: BoneWeights0;
	uint4 BoneIndexes	: BoneIndexes0;
	uint4 BoneIndexesOriginal	: BoneIndexesOriginal0;

	uint InstanceId		: SV_InstanceID;
};
//||>

struct VS_Output
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 ProjPosition		: PROJPOSITION0;
	float4 Color			: Color0;
	half2 UV				: UV0;
	half2 UVSub				: UVSub0;
	half3 Normal			: NORMAL0;
	half3 Binormal			: BINORMAL0;
	half3 Tangent			: TANGENT0;
};

//<|| モデル共通関数

//||>

Texture2D		g_animationTexture;
SamplerState	g_animationSampler;

float			g_animationTextureWidth;
float			g_animationTextureHeight;

float4x4 getMatrix(uint animationIndex, uint boneIndex, float time)
{
	float x = time / g_animationTextureWidth;
	uint yind = animationIndex * 32 * 4 + boneIndex * 4;
	float y0 = (yind + 0) / g_animationTextureHeight;
	float y1 = (yind + 1) / g_animationTextureHeight;
	float y2 = (yind + 2) / g_animationTextureHeight;
	float y3 = (yind + 3) / g_animationTextureHeight;

	float4 y0v = g_animationTexture.SampleLevel(g_animationSampler, float2(x,y0), 0);
	float4 y1v = g_animationTexture.SampleLevel(g_animationSampler, float2(x,y1), 0);
	float4 y2v = g_animationTexture.SampleLevel(g_animationSampler, float2(x,y2), 0);
	float4 y3v = g_animationTexture.SampleLevel(g_animationSampler, float2(x,y3), 0);

	return float4x4( y0v, y1v, y2v, y3v );
}

float4x4 calcMatrix(uint animationIndex, float time, float4 weights, uint4 indexes)
{
	return getMatrix(animationIndex, indexes.x, time) * weights.x +
	getMatrix(animationIndex, indexes.y, time) * weights.y +
	getMatrix(animationIndex, indexes.z, time) * weights.z +
	getMatrix(animationIndex, indexes.w, time) * weights.w;
}

float3x3 convert44to33(float4x4 mat)
{
	return (float3x3)mat;
}


VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	float animIndex0 = animationIndex0[Input.InstanceId];
	float animIndex1 = animationIndex1[Input.InstanceId];
	float animTime0 = animationTime0[Input.InstanceId];
	float animTime1 = animationTime1[Input.InstanceId];
	float animWeight = animationWeight[Input.InstanceId];

	float4x4 matLocal0 = mul( calcMatrix(animIndex0, animTime0, Input.BoneWeights,Input.BoneIndexes), matM[Input.InstanceId]);
	float4x4 matLocal1 = mul( calcMatrix(animIndex1, animTime1, Input.BoneWeights,Input.BoneIndexes), matM[Input.InstanceId]);
	float4x4 matLocal = matLocal0 * animWeight + matLocal1 * (1.0 - animWeight);

	matLocal = matM[Input.InstanceId];

	float4x4 matMC = mul(matC, matLocal);
	float3x3 matC33 = convert44to33(matC);
	float3x3 matMC33 = convert44to33(matMC);

	float4 cPosition = mul( matMC, float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ) );

	float3 cNormal = mul( matMC33, Input.Normal );
	cNormal = normalize(cNormal);

	float3 cBinormal = mul( matMC33, Input.Binormal );
	cBinormal = normalize(cBinormal);

	float3 cTangent = cross( cBinormal, cNormal );
	cTangent = normalize(cTangent);

	Output.SV_Position = mul( matP, cPosition );
	Output.Position = Output.SV_Position;
	Output.Normal = (half3)cNormal;
	Output.Binormal = (half3)cBinormal;
	Output.Tangent = (half3)cTangent;
	Output.UV = Input.UV;
	Output.UVSub = Input.UVSub;
	Output.Color = Input.Color;
	Output.ProjPosition = Output.SV_Position;

	return Output;
}

)";