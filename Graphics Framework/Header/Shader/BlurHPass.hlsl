//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D BrightPass : register(t0);

SamplerState samLinear : register(s0);

cbuffer blurHData : register(b0)
{
	float2 ViewportDimensions;
	int LevelOfDetail;
};

//--------------------------------------------------------------------------------------
//Constant defines                                
//--------------------------------------------------------------------------------------
const float2 OFFSETS[] =                     // Pixel sampler(Near pixels)
{
	 float2(-4, 0),
	 float2(-3, 0),
	 float2(-2, 0),
	 float2(-1, 0),
	 float2(0, 0),
	 float2(1, 0),
	 float2(2, 0),
	 float2(3, 0),
	 float2(4, 0)
};

static const float BlurWeights[] =         // Gauss distribution
{
	 0.004815026f,
	 0.028716039f,
	 0.102818575f,
	 0.221024189f,
	 0.285252340f,
	 0.221024189f,
	 0.102818575f,
	 0.028716039f,
	 0.004815026f
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
	float Texel = (1.0f / ViewportDimensions.x) * (LevelOfDetail + 1);

	float4
	Color =  BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[0] * Texel)), LevelOfDetail) * BlurWeights[0];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[1] * Texel)), LevelOfDetail) * BlurWeights[1];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[2] * Texel)), LevelOfDetail) * BlurWeights[2];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[3] * Texel)), LevelOfDetail) * BlurWeights[3];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[4] * Texel)), LevelOfDetail) * BlurWeights[4];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[5] * Texel)), LevelOfDetail) * BlurWeights[5];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[6] * Texel)), LevelOfDetail) * BlurWeights[6];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[7] * Texel)), LevelOfDetail) * BlurWeights[7];
	Color += BrightPass.SampleLevel(samLinear, float2(input.Tex + (OFFSETS[8] * Texel)), LevelOfDetail) * BlurWeights[8];
	
	return Color;
}