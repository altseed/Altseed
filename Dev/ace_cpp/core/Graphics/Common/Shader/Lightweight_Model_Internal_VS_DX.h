static const char* lightweight_model_internal_vs_dx = R"(

//<|| モデル共通レジスタ
float4x4	matM[32]					: register( c0 );
float4x4	matC						: register( c128 );
float4x4	matP						: register( c132 );
//||>

float3		directionalLightDirection	: register( c136 );
float3		directionalLightColor		: register( c137 );
float3		skyLightColor				: register( c138 );
float3		groundLightColor			: register( c139 );

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
	return float3x3(mat[0].xyz, mat[1].xyz, mat[2].xyz);
}
//||>

float4 calcLightColor(float3 upDir, float3 normal, float3 lightDir)
{
	float4 color = float4(0.0,0.0,0.0,1.0);
	float NoL = dot(normal,lightDir);
	float NoU = dot(normal,upDir);
	color.xyz = directionalLightColor * max(NoL,0.0);
	color.xyz = color.xyz + skyLightColor * max(NoU,0.0);
	color.xyz = color.xyz + groundLightColor * max(-NoU,0.0);
	return color;
}

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	float4x4 matLocal = calcMatrix(Input.BoneWeights,Input.BoneIndexes);
	float4x4 matMC = mul(matC, matLocal);
	float4x4 matMLC = mul(matLC, matLocal);
	float3x3 matC33 = convert44to33(matC);
	float3x3 matMC33 = convert44to33(matMC);

	float4 cPosition = mul( matMC, float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ) );

	float3 cNormal = mul( matMC33, Input.Normal );
	cNormal = normalize(cNormal);

	float3 cDirectionalLightDirection = mul(matC33, directionalLightDirection).xyz;
	
	float3 upDir = mul(matC33, float3(0.0,1.0,0.0));

	Output.SV_Position = mul( matP, cPosition );
	Output.Position = Output.SV_Position;
	Output.Normal = (half3)cNormal.xyz;
	Output.UV = Input.UV;
	Output.Color = Input.Color * calcLightColor(upDir,cNormal,cDirectionalLightDirection);

	return Output;
}

)";