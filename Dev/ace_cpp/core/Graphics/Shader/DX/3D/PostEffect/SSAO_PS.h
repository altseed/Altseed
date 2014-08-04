static const char* ssao_ps_dx = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float radius		: register( c0 );

// 1mの位置にサイズ1のオブジェクトを出した時の縦方向ピクセル数
float projScale		: register( c1 );
float bias			: register( c2 );
float intensity		: register( c3 );

// 座標再構成情報
// perspective
// x = n・f
// y = f-n
// z = -f

// ace
// x = f-n
// y = n
float3 reconstructInfo1	: register( c4 );

// x = 1/xScale
// y = 1/yScale
float4 reconstructInfo2	: register( c5 );


struct PS_Input
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

// サンプル数
#define NUM_SAMPLES (13)

// サンプル時の回転数
#define NUM_TURNS (7)


float3 GetNormal(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).xyz;
}

float GetZ(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).w;
}

float ReconstructDepth(float z)
{
	return reconstructInfo1.x / (reconstructInfo1.y * z + reconstructInfo1.z);
	//return -((z * reconstructInfo1.x) + reconstructInfo1.y);
}

float3 ReconstructPosition(float2 screenXY, float depth)
{
	return float3( reconstructInfo2.xy * screenXY * (-depth), depth );
}

float2 GetScreenPos(float2 uv)
{
	return float2(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0 );
}

int2 GetScreenPixelPos(float2 uv)
{
	uint width, height;
	g_texture.GetDimensions(width, height);
	return int2(width*uv.x,height*uv.y);
}

// 球内のサンプルする位置のオフセットをユニット単位で取得する。
float3 GetSphereOffset(int index, float offset)
{
	float alpha = float(index + 0.5) * (1.0 / NUM_SAMPLES);
	float angle = alpha * (NUM_TURNS * 6.28) + offset;

	return float3(cos(angle), sin(angle), alpha);
}


float4 SampleAO(float3 centerPos, float2 centerUV, float3 normal, float sRadius, float random, int index)
{
	uint width, height;
	g_texture.GetDimensions(width, height);

	float3 offset = GetSphereOffset(index, random );

	float2 targetUV = centerUV;
	targetUV.x = targetUV.x + offset.x * sRadius * offset.z / (float)width;
	targetUV.y = targetUV.y + offset.y * sRadius * offset.z / (float)height;

	float3 targetPos = ReconstructPosition( GetScreenPos(targetUV), ReconstructDepth(GetZ(targetUV)));
	
	float3 v = targetPos - centerPos;

	float vv = dot(v, v);
	float vn = dot(v, normal);

	// Scalable Ambient Obscurance記載
	const float epsilon = 0.01;
    float f = max(radius * radius - vv, 0.0);

	return f * f * f * max((vn - bias) / (epsilon + vv), 0.0);
}

float2 CompressValue(float value)
{
	float temp = floor(value * 256.0);
	return float2( temp * (1.0 / 256.0), value * 256.0 - temp );
}


float4 main( const PS_Input Input ) : SV_Target
{
	int2 sPos = GetScreenPixelPos(Input.UV);
	float3 centerPos = ReconstructPosition( GetScreenPos(Input.UV), ReconstructDepth(GetZ(Input.UV)));

	// 中心の奥行きでの球の半径を計算(カメラ座標系のため、zはマイナス)
	float sRadius = -projScale * radius / centerPos.z;

	// Scalable Ambient Obscurance記載のランダム
	float random = (3 * sPos.x ^ sPos.y + sPos.x * sPos.y) * 10;

	float3 normal =  GetNormal(Input.UV);

	float sum = 0.0;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		sum += SampleAO(centerPos, Input.UV, normal, sRadius, random, i);
	}

	// Scalable Ambient Obscurance記載のAの強さの算出方法
	float A = max( 0.0, 1.0 - intensity * sum * 5.0 / (radius * radius * radius * radius * radius * radius * NUM_SAMPLES) );

	// Bilateral box-filter
	if (abs(ddx(centerPos.z)) < 0.02)
	{
		A -= ddx(A) * ((sPos.x & 1) - 0.5);
	}
	if (abs(ddy(centerPos.z)) < 0.02)
	{
		A -= ddy(A) * ((sPos.y & 1) - 0.5);
	}

	float2 compressedDepth = CompressValue(centerPos.z);
	return float4(A,compressedDepth.x,compressedDepth.y,1.0);
}

)";