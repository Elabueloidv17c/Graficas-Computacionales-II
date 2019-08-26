//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D LightingPass : register(t0);
Texture2D LuminancePass : register(t1);

SamplerState samLinear : register(s0);

cbuffer brightData : register(b0)
{
	int LevelOfDetail;
	float BloomThreshold;
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
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 Color = LightingPass.Sample(samLinear, input.Tex);
	float Brigth = LuminancePass.SampleLevel(samLinear, input.Tex, LevelOfDetail).x;

	Brigth = max(Brigth - BloomThreshold, 0.0f);

	return Color * Brigth;
}