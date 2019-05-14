#define PIXEL_LIGHT
//#define BLINN_PHONG

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);

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
	float4 DiffuseColor;
	float4 SpecularColor;
	float4 AmbientColor;
	float DiffuseIntensity;
	float SpecularIntensity;
	float AmbientIntensity;
};

cbuffer lighting : register(b4)
{
	float3 DirectionalLight;
	float SpecularPower;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Nor : NORMAL;
	float3 Tan : TANGENT;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;

#ifdef PIXEL_LIGHT
	float3 Nor : NORMAL;
	float3 Tan : TANGENT;
	float3 PosWS : WS_POSITION;
#else
	float4 Col : COLOR;
#endif
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

#ifdef PIXEL_LIGHT
	output.PosWS = input.Pos;
	output.Nor = mul(float4(input.Nor.xyz, 0), Model).xyz;
	output.Tan = mul(float4(input.Tan.xyz, 0), Model).xyz;
#else
	// Lambert light calc for Normals---------------------------------------
	float3 NormalWS = normalize(mul(float4(input.Nor.xyz, 0), Model)).xyz;
	float3 TangentWS = normalize(mul(float4(input.Tan.xyz, 0.0), Model)).xyz;
	
	float3 lightDirectionWS = -normalize(DirectionalLight.xyz);
	
	float NdLWS = max(0.0f, dot(lightDirectionWS, NormalWS.xyz));

	float3 cameraPos = float3(View[0][3], View[1][3], View[2][3]);
	float3 viewDirectionWS = -normalize(input.Pos.xyz - cameraPos);
	
	#ifdef BLINN_PHONG
		float3 HalfVector = normalize(viewDirectionWS + lightDirectionWS);
		float NdHWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
		float specularFactor = pow(NdHWS, SpecularPower) * NdLWS;
	#else
		float3 reflectWS = normalize(reflect(-lightDirectionWS.xyz, NormalWS.xyz));
		float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));
		float specularFactor = pow(viewReflectionWS, SpecularPower) * NdLWS;
	#endif
	
	// Light aportation
	float3 Diffuse = DiffuseIntensity * DiffuseColor * NdLWS;
	float3 Specular = SpecularIntensity * SpecularColor * specularFactor;
	float3 Ambient = AmbientIntensity * (1.0 - NdLWS) * AmbientColor;
	
	output.Col.xyz = Ambient.xyz + Diffuse.xyz + Specular.xyz;
#endif

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
#ifdef PIXEL_LIGHT
	//Pixel Normal
	float3 BinormalWS = normalize(cross(input.Nor, input.Tan));
	
	float3 TexNormal = txNormal.Sample(samLinear, input.Tex).xyz;
	TexNormal.xyz = normalize(2.0f * TexNormal.xyz - 1.0f);
	
	float3x3 TBNMatrix = float3x3(input.Tan, BinormalWS, input.Nor);
	float3 NormalWS = normalize(mul(TexNormal, TBNMatrix));
	
	// Lambert light calc
	float3 LightDirectionWS = -normalize(DirectionalLight.xyz);
	float NdLWS = max(0.0f, dot(LightDirectionWS.xyz, input.Nor.xyz));
	
	// Blinn specular
	float3 cameraPos = float3(View[0][3], View[1][3], View[2][3]);
	float3 ViewDirectionWS = -normalize(input.PosWS.xyz - cameraPos.xyz);

	#ifdef BLINN_PHONG
		float3 HalfVector = -normalize(ViewDirectionWS + LightDirectionWS);
		float NdHWS = max(0.0f, dot(HalfVector.xyz, NormalWS.xyz));
		float specularFactor = pow(NdHWS, SpecularPower) * NdLWS;
	#else
		float3 ReflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));
		float VdRWS = max(0.0f, dot(ViewDirectionWS.xyz, ReflectWS.xyz));
		float specularFactor = pow(VdRWS, SpecularPower) * NdLWS;
	#endif

	float3 Diffuse = DiffuseIntensity * DiffuseColor * NdLWS;
	float3 Specular = SpecularIntensity * SpecularColor * specularFactor;
	float3 Ambient = AmbientIntensity * (1.0 - NdLWS) * AmbientColor;

	return txDiffuse.Sample(samLinear, input.Tex) * float4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);
	//return float4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);
#else
	return txDiffuse.Sample(samLinear, input.Tex) * input.Col;
#endif

}
