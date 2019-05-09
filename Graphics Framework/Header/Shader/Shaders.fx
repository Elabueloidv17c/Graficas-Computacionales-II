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

	float3 PosWS : WS_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	output.PosWS = input.Pos;

	output.Pos = mul(input.Pos, Model);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	
	output.Nor = mul(float4(input.Nor.xyz, 0), Model);
	output.Tex = input.Tex;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Lambert light calc
	float3 LightDirectionWS = -normalize(Directional.xyz);
	float NdLWS = max(0.0f, dot(LightDirectionWS.xyz, input.Nor.xyz));

	// Blinn specular
	float3 cameraPos = float3(View[0][3], View[1][3], View[2][3]);
	float3 ViewDirectionWS = -normalize(input.PosWS.xyz - cameraPos.xyz);
	
	float3 ReflectWS = normalize(reflect(-LightDirectionWS.xyz, input.Nor.xyz));
	
	float VdRWS = max(0.0f, dot(ViewDirectionWS.xyz, ReflectWS.xyz));
	
	float specularFactor = pow(VdRWS, SpecularPower) * NdLWS;

	return txDiffuse.Sample(samLinear, input.Tex) * float4((NdLWS.xxx * MeshColor.xyz) + (specularFactor.xxx * SpecularColor.xyz), 1);
}
