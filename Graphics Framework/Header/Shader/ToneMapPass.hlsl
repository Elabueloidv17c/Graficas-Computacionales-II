//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D LightingPass : register(t0);
Texture2D AddBrightPass : register(t1);

SamplerState samLinear : register(s0);

cbuffer toneMapPass : register(b0)
{
	float Exposure;
	float BloomMultiplier;
};

//--------------------------------------------------------------------------------------
// Shader return values
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
	float4 Pos			: SV_POSITION;
	float2 Tex			: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Helper Functions
//--------------------------------------------------------------------------------------
float3 BasicExposure(float3 Color)
{
	Color *= Exposure;
	Color = pow(Color, 1.0f / 2.2f);

	return Color;
}

float3 Reinhard(float3 Color)
{
	Color *= Exposure;
	Color /= (1.0f + Color);
	Color = pow(Color, 1.0f / 2.2f);

	return Color;
}

float3 Burgess_Dawson(float3 Color)
{
	Color *= Exposure;
	float3 x = max(0, Color - 0.004f);
	Color = (x * (6.2 * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

	return Color;
}

float3 Uncharted2Tonemap(float3 x)
{
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;

	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float3 Uncharted2(float3 Color)
{
	float W = 11.2f;
	Color *= Exposure;
	float ExposureBias = 2.0f;
	float3 curr = Uncharted2Tonemap(ExposureBias * Color);

	float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
	float3 color = curr * whiteScale;

	Color = pow(Color, 1 / 2.2);
	return Color;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Take color 
	float3 Color = LightingPass.Sample(samLinear, input.Tex.xy).xyz;

	// Add HDR
	Color += BloomMultiplier * AddBrightPass.Sample(samLinear, input.Tex.xy).xyz;
	Color = Uncharted2(Color);

	//// Take the linear color to standard RGBA color
	//if (input.Tex.x <= 0.5f && input.Tex.y <= 0.5f)
	//{
	//	Color = BasicExposure(Color);
	//}
	//else if (input.Tex.x <= 0.5f && input.Tex.y > 0.5f)
	//{
	//	Color = Uncharted2(Color);
	//}
	//else if (input.Tex.x > 0.5f && input.Tex.y <= 0.5f)
	//{
	//	Color = Burgess_Dawson(Color);
	//}
	//else if (input.Tex.x > 0.5f && input.Tex.y > 0.5f)
	//{
	//	Color = Reinhard(Color);
	//}
	
	return float4(Color, 1.0f);
}


//float3 Haarm_Peter_Duiker(in float3 Color, in float exposure)
//{
	//float3 texColor = Color;
	//texColor *= exposure;
	//
	//float3 ld = 0.002f;
	//float linReference = 0.18;
	//float logReference = 444;
	//float logGamma = 0.45f;
	//
	//float3 LogColor;
	//LogColor.rgb = (log10(0.4f * texColor.rgb / linReference) / ld * logGamma + logReference) / 1023.f;
	//LogColor.rgb = saturate(LogColor.rgb);
	//
	//float FilmLutWidth = 256;
	//float Padding = 0.5f / FilmLutWidth;
	//
	//float3 retColor;
	//retColor.x = tex2D(Light, float2(lerp(Padding, 1 - Padding, LogColor.x), 0.5f)).r;
	//retColor.y = tex2D(Light, float2(lerp(Padding, 1 - Padding, LogColor.y), 0.5f)).r;
	//retColor.z = tex2D(Light, float2(lerp(Padding, 1 - Padding, LogColor.z), 0.5f)).r;
	//
	//return retColor;
//}