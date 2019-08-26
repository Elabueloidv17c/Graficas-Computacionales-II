//--------------------------------------------------------------------------------------
// Shader return values
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION0;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = float4(input.Pos.xyz, 1.0f);
	output.Tex = float2(1.0f - input.Tex.x, 1.0f - input.Tex.y);

	return output;
}