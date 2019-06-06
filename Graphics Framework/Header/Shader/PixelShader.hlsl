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
	float4 PointColor;
	float4 SpotColor;

	float4 DiffuseColor;
	float4 SpecularColor;
	float4 AmbientColor;

	float DiffuseIntensity;
	float SpecularIntensity;
	float AmbientIntensity;

};

cbuffer lighting : register(b4)
{
	float3	DirectionalLight;
	float	SpecularPower;

	float3	PointPosition;
	float   PointRadius;

	float3	ViewPosition;
	float   SpotRadius;

	float3	SpotPosition;
	float   SpotAlpha;

	float3	SpotLightDirection;
	float   SpotBeta;
};

//--------------------------------------------------------------------------------------
// Shader return values
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Nor : NORMAL;
	float3 Tan : TANGENT;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;

#ifdef PIXEL_LIGHT
	float3 Nor	 : NORMAL;
	float3 Tan	 : TANGENT;
	float4 PosWS : WS_POS;
	matrix Model : MODEL_MATRIX;
#else
	float4 Col : COLOR;
#endif
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
#ifdef PIXEL_LIGHT

	float4 PosWS = input.PosWS;

	//Pixel Normal
	float3 BinormalWS = normalize(cross(input.Nor, input.Tan));
	float3 TexNormal = txNormal.Sample(samLinear, input.Tex).xyz;
	TexNormal.xyz = normalize(2.0f * TexNormal.xyz - 1.0f);
	float3x3 TBNMatrix = float3x3(input.Tan, BinormalWS, input.Nor);
	float3 NormalWS = normalize(mul(TexNormal, TBNMatrix));

	float3 ViewDirectionWS;
	float3 LightDirectionWS;
	float NormalDotLightWS;
	float SpecularFactor;

	float3 PointViewDirectionWS;
	float3 PointLightDirectionWS;
	float PointNormalDotLightWS;
	float PointSpecularFactor;

	float3 SpotViewDirectionWS;
	float3 SpotLightDirectionWS;
	float SpotNormalDotLightWS;
	float SpotSpecularFactor;

	float3 Distance;

	LightDirectionWS = -normalize(DirectionalLight.xyz);
	ViewDirectionWS = -normalize(DirectionalLight.xyz);
	NormalDotLightWS = max(0.0f, dot(LightDirectionWS, NormalWS.xyz));

#ifdef POINT_LIGHT
	PointLightDirectionWS = -normalize(PosWS.xyz - mul(float4(PointPosition.xyz, 1), input.Model).xyz);
	PointViewDirectionWS = PointLightDirectionWS;
	Distance = length(PosWS.xyz - mul(float4(PointPosition.xyz, 1), input.Model).xyz);
	PointNormalDotLightWS = max(0.0f, dot(PointViewDirectionWS.xyz, NormalWS.xyz)) * (PointRadius / Distance);
#endif
#ifdef SPOT_LIGHT
	SpotViewDirectionWS = -normalize(PosWS - ViewPosition);
	SpotLightDirectionWS = -normalize(SpotLightDirection.xyz);

	float3 SpotDirToVertex = -normalize(PosWS.xyz - ViewPosition.xyz);
	float Theta = dot(SpotDirToVertex, SpotLightDirectionWS.xyz);
	float Spot = Theta - cos(SpotBeta * 0.5);
	Spot = max(0.0, Spot / (cos(SpotAlpha * 0.5) - cos(SpotBeta * 0.5)));

	Distance = length(PosWS.xyz - ViewPosition.xyz);
	SpotNormalDotLightWS = max(0.0, dot(SpotLightDirectionWS, NormalWS.xyz) * Spot) * (SpotRadius / Distance);
#endif
#ifdef BLINN_PHONG
	float3 HalfVector = normalize(ViewDirectionWS.xyz + LightDirectionWS.xyz);
	float NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
	SpecularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;

	HalfVector = normalize(PointViewDirectionWS.xyz + PointLightDirectionWS.xyz);
	NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
	PointSpecularFactor = pow(NormalDotHalfWS, SpecularPower) * PointNormalDotLightWS;

	HalfVector = normalize(SpotViewDirectionWS.xyz + SpotLightDirectionWS.xyz);
	NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
	SpotSpecularFactor = pow(NormalDotHalfWS, SpecularPower) * SpotNormalDotLightWS;
#else
	float3 ReflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));
	float ViewReflectionWS = max(0.0f, dot(ViewDirectionWS, ReflectWS));
	SpecularFactor = pow(ViewReflectionWS, SpecularPower) * NormalDotLightWS;

	ReflectWS = normalize(reflect(-PointLightDirectionWS.xyz, NormalWS.xyz));
	ViewReflectionWS = max(0.0f, dot(PointViewDirectionWS, ReflectWS));
	PointSpecularFactor = pow(ViewReflectionWS, SpecularPower) * PointNormalDotLightWS;

	ReflectWS = normalize(reflect(-SpotLightDirectionWS.xyz, NormalWS.xyz));
	ViewReflectionWS = max(0.0f, dot(SpotViewDirectionWS, ReflectWS));
	SpotSpecularFactor = pow(ViewReflectionWS, SpecularPower) * SpotNormalDotLightWS;
#endif

	// Light aportation
	float3 Diffuse = DiffuseIntensity * DiffuseColor * NormalDotLightWS;
	float3 Specular = SpecularIntensity * SpecularColor * SpecularFactor;
	float3 Ambient = AmbientIntensity * (1.0 - NormalDotLightWS);

#ifdef POINT_LIGHT
	Diffuse += DiffuseIntensity * PointColor * PointNormalDotLightWS;
	Specular += SpecularIntensity * SpecularColor * PointSpecularFactor;
	Ambient *= (1.0 - PointNormalDotLightWS);
#endif
#ifdef SPOT_LIGHT
	Diffuse += DiffuseIntensity * SpotColor * SpotNormalDotLightWS;
	Specular += SpecularIntensity * SpecularColor * SpotSpecularFactor;
	Ambient *= (1.0 - SpotNormalDotLightWS);
#endif
	//return float4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);
	return txDiffuse.Sample(samLinear, input.Tex) * float4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);
#else
	//return input.Col;
	return txDiffuse.Sample(samLinear, input.Tex) * input.Col;
#endif
}