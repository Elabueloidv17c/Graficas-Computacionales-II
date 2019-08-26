//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
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
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Tex = input.Tex;

#ifdef PIXEL_LIGHT
	output.PosWS = mul(input.Pos, Model);
	output.Model = Model;

	output.Nor = mul(float4(input.Nor.xyz, 0), Model).xyz;
	output.Tan = mul(float4(input.Tan.xyz, 0), Model).xyz;

	output.Pos = mul(float4(input.Pos, 1), Model);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

#else
	float4 PosWS = mul(input.Pos, Model);

	output.Pos = mul(float4(input.Pos, 1), Model);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	float3 NormalWS = normalize(mul(float4(input.Nor.xyz, 0), Model)).xyz;

	float3 Distance;

	float3 LightDirectionWS = -normalize(DirectionalLight.xyz);
	float3 ViewDirectionWS = -normalize(DirectionalLight.xyz);
	float NormalDotLightWS = max(0.0f, dot(LightDirectionWS, NormalWS.xyz));

	#ifdef POINT_LIGHT
		float3 PointLightDirectionWS = -normalize(PosWS.xyz - mul(float4(PointPosition.xyz, 1), Model).xyz);
		float3 PointViewDirectionWS = PointLightDirectionWS;
		Distance = length(PosWS.xyz - mul(float4(PointPosition.xyz, 1), Model).xyz);
		float PointNormalDotLightWS = max(0.0f, dot(PointViewDirectionWS.xyz, NormalWS.xyz)) * (PointRadius / Distance);
	#endif
	#ifdef SPOT_LIGHT
		float3 SpotViewDirectionWS = -normalize(PosWS - ViewPosition);
		float3 SpotLightDirectionWS = -normalize(SpotLightDirection.xyz);

		float3 SpotDirToVertex = -normalize(PosWS.xyz - ViewPosition.xyz);
		float Theta = dot(SpotDirToVertex, SpotLightDirectionWS.xyz);
		float Spot = Theta - cos(SpotBeta * 0.5);
		Spot = max(0.0, Spot / (cos(SpotAlpha * 0.5) - cos(SpotBeta * 0.5)));
		
		Distance = length(PosWS.xyz - ViewPosition.xyz);
		float SpotNormalDotLightWS = max(0.0, dot(SpotLightDirectionWS, NormalWS.xyz) * Spot) * (SpotRadius / Distance);
	#endif
	#ifdef BLINN_PHONG
		float3 HalfVector = normalize(ViewDirectionWS.xyz + LightDirectionWS.xyz);
		float NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
		float SpecularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;

		#ifdef POINT_LIGHT
			HalfVector = normalize(PointViewDirectionWS.xyz + PointLightDirectionWS.xyz);
			NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
			float PointSpecularFactor = pow(NormalDotHalfWS, SpecularPower) * PointNormalDotLightWS;
		#endif
		#ifdef SPOT_LIGHT
			HalfVector = normalize(SpotViewDirectionWS.xyz + SpotLightDirectionWS.xyz);
			NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
			float SpotSpecularFactor = pow(NormalDotHalfWS, SpecularPower) * SpotNormalDotLightWS;
		#endif
	#else
		float3 ReflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));
		float ViewReflectionWS = max(0.0f, dot(ViewDirectionWS, ReflectWS));
		float SpecularFactor = pow(ViewReflectionWS, SpecularPower) * NormalDotLightWS;

		#ifdef POINT_LIGHT
			ReflectWS = normalize(reflect(-PointLightDirectionWS.xyz, NormalWS.xyz));
			ViewReflectionWS = max(0.0f, dot(PointViewDirectionWS, ReflectWS));
			float PointSpecularFactor = pow(ViewReflectionWS, SpecularPower) * PointNormalDotLightWS;
		#endif
		#ifdef SPOT_LIGHT
			ReflectWS = normalize(reflect(-SpotLightDirectionWS.xyz, NormalWS.xyz));
			ViewReflectionWS = max(0.0f, dot(SpotViewDirectionWS, ReflectWS));
			float SpotSpecularFactor = pow(ViewReflectionWS, SpecularPower) * SpotNormalDotLightWS;
		#endif
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

	output.Col.xyz = Ambient.xyz + Diffuse.xyz + Specular.xyz;
#endif
	return output;
}