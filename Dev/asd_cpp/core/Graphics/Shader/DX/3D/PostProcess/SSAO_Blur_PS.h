static const char* ssao_blur_ps_dx = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

struct PS_Input
{
	float4 SV_Position		: SV_POSITION;
	float4 Position			: POSITION0;
	float2 UV				: UV0;
};

float GetValue(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).x;
}

float2 GetCompressedDepth(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).yz;
}

float DecompressValue(float2 compressed)
{
	return compressed.x * (255.0/256.0) + compressed.y * (1.0 / 256.0);
}

float GetDepth(float2 uv)
{
	float2 compressed = GetCompressedDepth(uv);
	return DecompressValue(compressed);
}

float4 main( const PS_Input Input ) : SV_Target
{
	const int radius = 4;
	const float scale = 2.0;
	const float centerOffset = 0.5;
	const float gaussian[] = { 0.144760504, 0.129537389, 0.103725441, 0.0743225217, 0.0476541445 };
	const float intensity = 200.0;

	uint width, height;
	g_texture.GetDimensions(width, height);
	float scaleX = scale / (float)width;
	float scaleY = scale / (float)height;
	float centerOffsetX = centerOffset / (float)width;
	float centerOffsetY = centerOffset / (float)height;

	float baseDepth = GetDepth( Input.UV );

	float sum = GetValue( Input.UV ) * gaussian[0];
	float weightSum = gaussian[0];

	[unroll]
	for(int r = -radius; r <= radius; r++)
	{
		if(r != 0)
		{
	#if BLUR_X
			float2 uv = Input.UV + float2(r * scaleX + centerOffsetX, 0.0 + centerOffsetY);
	#endif
	
	#if BLUR_Y
			float2 uv = Input.UV + float2(0.0 + centerOffsetX, r * scaleY + centerOffsetY);
	#endif
	
			float depth = GetDepth( uv );
			
			float weight =  gaussian[abs(r)];
			weight = weight * max(0.0, 1.0 - intensity * abs(depth-baseDepth) );
			
			float value = GetValue(uv);
			
			sum += value * weight;
			weightSum += weight;
		}
	}

	const float epsilon = 0.0001;
	float a = (sum + epsilon) / (weightSum + epsilon);

	return float4(a, GetCompressedDepth(Input.UV), 1.0 );
}

)";