static const char* lightweight_terrain_internal_vs_dx = R"(

//<|| モデル共通レジスタ
float4x4	matM						: register( c0 );
float4x4	matC						: register( c128 );
float4x4	matP						: register( c132 );
//||>

#ifndef BLACK
float3		directionalLightDirection	: register( c136 );
float3		directionalLightColor		: register( c137 );
float3		skyLightColor				: register( c138 );
float3		groundLightColor			: register( c139 );
#endif

//<|| モデル共通頂点入力
struct VS_Input
{
	float3 Position		: Position0;
	float3 Normal		: Normal0;
	float3 Binormal		: Binormal0;
	float2 UV			: UV0;
	float2 UVSub		: UVSub0;
	float4 Color		: Color0;
};
//||>

struct VS_Output
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 Color			: Color0;
	half2 UV				: UV0;
	half2 UVSub				: UVSub0;
	half3 Normal			: NORMAL0;
};

//<|| モデル共通関数
float3x3 convert44to33(float4x4 mat)
{
	return (float3x3)mat;
}
//||>

#ifndef BLACK
float4 calcLightColor(float3 upDir, float3 normal, float3 lightDir)
{
	float4 color = float4(0.0,0.0,0.0,1.0);
	float NoL = dot(normal,lightDir);
	float NoU = dot(normal,upDir);
	color.xyz = directionalLightColor * max(NoL,0.0);
	color.xyz = color.xyz + skyLightColor * max(NoU+1,0.0) / 2.0;
	color.xyz = color.xyz + groundLightColor * max(-NoU+1,0.0) / 2.0;
	return color;
}
#endif

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	float4x4 matLocal = matM;
	float4x4 matMC = mul(matC, matLocal);
	float3x3 matC33 = convert44to33(matC);
	float3x3 matMC33 = convert44to33(matMC);

	float4 cPosition = mul( matMC, float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ) );

	float3 cNormal = mul( matMC33, Input.Normal );
	cNormal = normalize(cNormal);

#ifndef BLACK
	float3 cDirectionalLightDirection = mul(matC33, directionalLightDirection).xyz;
#endif

	float3 upDir = mul(matC33, float3(0.0,1.0,0.0));

	Output.SV_Position = mul( matP, cPosition );
	Output.Position = Output.SV_Position;
	Output.Normal = (half3)cNormal.xyz;
	Output.UV = Input.UV;
	Output.UVSub = Input.UVSub;

#ifdef BLACK
	Output.Color = Input.Color;
#else
	Output.Color = Input.Color * calcLightColor(upDir,cNormal,cDirectionalLightDirection);
#endif

	return Output;
}

)";