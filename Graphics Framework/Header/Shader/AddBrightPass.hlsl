//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D BrightPass : register(t0);
Texture2D BlurHPass : register(t1);
Texture2D BlurVPass : register(t2);

SamplerState samLinear : register(s0);

cbuffer addBrightData : register(b0)
{
	int LevelOfDetail;
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
	float4 Bright = BrightPass.Sample(samLinear, input.Tex);

	float4 BlurH = BlurHPass.SampleLevel(samLinear, input.Tex, LevelOfDetail + 1);
	float4 BlurV = BlurVPass.SampleLevel(samLinear, input.Tex, LevelOfDetail + 1);

	return (0.5f * (BlurH + BlurV)) + Bright;
}