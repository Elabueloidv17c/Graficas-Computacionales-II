//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer modelMatrix : register(b0)
{
	matrix Model;
};

cbuffer viewMatrix : register(b1)
{
	matrix View;
};

cbuffer projectionMatrix : register(b2)
{
	matrix Projection;
};

cbuffer color : register(b3)
{
	float4 MeshColor;
	float4 SpecularColor;
};

cbuffer lighting : register(b4)
{
	float3 Directional;
	float SpecularPower;
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

	float4 Col : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = mul(input.Pos, Model);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Tex = input.Tex;

	// Lambert light calc for Normals---------------------------------------
	float4 NormalWS = mul(float4(input.Nor.xyz, 0), Model);

	float3 lightDirectionWS = -normalize(Directional.xyz);

	float NdLWS = dot(lightDirectionWS, NormalWS.xyz);
	output.Nor = NdLWS.xxx;

	// Blinn specular for MeshColor------------------------------------------

	float3 cameraPos = float3(View[0][3], View[1][3], View[2][3]);
	float3 viewDirectionWS = -normalize(input.Pos.xyz - cameraPos);

	float3 reflectWS = normalize(reflect(-lightDirectionWS.xyz, NormalWS.xyz));
	float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));

	float specularFactor = pow(viewReflectionWS, SpecularPower) * NdLWS;

	float3 finalColor = (NdLWS.xxx * MeshColor.xyz) + (specularFactor.xxx * SpecularColor.xyz);
	output.Col = float4(finalColor.xyz, 1);

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	return txDiffuse.Sample(samLinear, input.Tex) * input.Col;
}
