//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer viewMatrix : register(b0)
{
	matrix View;
};

cbuffer projectionMatrix : register(b1)
{
	matrix Projection;
};

cbuffer worldMatrix : register(b2)
{
	matrix World;
};

cbuffer color : register(b3)
{
	float4 MeshColor;
};

cbuffer lighting : register(b4)
{
	float3 Directional;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Nor : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Nor : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Tex = input.Tex;

	//Normal WorldSpace
	float3 NormalWS = mul(float4(input.Nor.xyz, 0), World);
	//Lambert light calc
	float3 lightDirectionWS = normalize(-Directional.xyz);
	//Dot Product
	float NdL = dot(lightDirectionWS, NormalWS);
	output.Nor = NdL.xxx;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 Normals = float4(input.Nor.xyz, 1);
	return txDiffuse.Sample(samLinear, input.Tex) * MeshColor * Normals;
}
