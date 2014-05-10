static const char* model_internal_vs_dx = R"(

//<|| モデル共通レジスタ
float4x4	matM[32]					: register( c0 );
float4x4	matC						: register( c128 );
float4x4	matP						: register( c132 );
//||>

float3		depthParams					: register( c136 );

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
};
//||>

struct VS_Output
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 Color			: Color0;
	half2 UV				: TEXCOORD0;
	half3 Normal			: NORMAL0;
	half3 Binormal			: BINORMAL0;
	half3 Tangent			: TANGENT0;
	float Depth				: DEPTH0;
};

//<|| モデル共通関数
float4x4 calcMatrix(float4 weights, uint4 indexes)
{
	return matM[indexes.x] * weights.x +
	matM[indexes.y] * weights.y +
	matM[indexes.z] * weights.z +
	matM[indexes.w] * weights.w;
}

float3x3 convert44to33(float4x4 mat)
{
	return (float3x3)mat;
}
//||>


VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	float4x4 matLocal = calcMatrix(Input.BoneWeights,Input.BoneIndexes);
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
	Output.Color = Input.Color;
	Output.Depth = (-cPosition.z - depthParams.z) / depthParams.x;

	return Output;
}

)";