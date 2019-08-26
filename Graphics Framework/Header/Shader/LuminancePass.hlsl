//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D LightingPass : register(t0);

SamplerState samLinear : register(s0);

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
	float4 LuminanceFactor = float4(0.2f, 0.587f, 0.15f, 1.0f);

	float4 Color = LightingPass.Sample(samLinear, input.Tex);

	float Luminance = dot(Color, LuminanceFactor);
	Luminance = log(Luminance + 0.000001f);
	
	return Luminance.xxxx;
}