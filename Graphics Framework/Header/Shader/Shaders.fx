//#define PIXEL_LIGHT
//#define BLINN_PHONG

//#define POINT_LIGHT
#define SPOT_LIGHT

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
	float3	DirectionalLight;
	float3	LightPosition;
	float3	LightDirection;

	float	SpecularPower;

	float   Constant;
	float   Linear;
	float   Quadratic;

	float   CutOff;
	float   OuterCutOff;
};

//--------------------------------------------------------------------------------------
// Shader return values
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
	#ifdef POINT_LIGHT

		float3 NormalWS = mul(mul(input.Nor, Model), View);
		float3 LightPositionWS = mul(mul(input.Pos, Model), View);
		float3 LightDirectionWS = -normalize(LightPositionWS);
		float3 viewDirectionWS = normalize(LightDirection);

		float NormalDotLightWS = max(0.0f, dot(LightDirectionWS, NormalWS.xyz));

		float distanceWS = length(mul(LightPosition, View) - mul(mul(input.Pos, Model), View));
		float Attenuation = (1000.0 / (Constant + (Linear * distanceWS) + (Quadratic * (distanceWS * distanceWS))));

		#ifdef BLINN_PHONG
			float3 HalfVector = normalize(viewDirectionWS + LightDirectionWS);
			float NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
			float specularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;
		#else
			float3 reflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));
			float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));
			float specularFactor = pow(viewReflectionWS, SpecularPower) * NormalDotLightWS;
		#endif

		// Light aportation
		float3 Diffuse = DiffuseIntensity * DiffuseColor * NormalDotLightWS * Attenuation;
		float3 Ambient = AmbientIntensity * (1.0 - NormalDotLightWS) * AmbientColor * Attenuation;
		float3 Specular = SpecularIntensity * SpecularColor * specularFactor * Attenuation;

		output.Col.xyz = Diffuse.xyz + Ambient.xyz + Specular.xyz;

	#elif defined (SPOT_LIGHT)

	float3 NormalWS = mul(mul(input.Nor, Model), View);
	
	float3 LightPositionWS = mul(mul(input.Pos, Model), View);
	float3 LightDirectionWS = -normalize(LightPositionWS);
	
	float3 viewDirectionWS = normalize(LightDirection);
	float3 reflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));

	float NormalDotLightWS = max(0.0f, dot(viewDirectionWS, NormalWS.xyz));
	
	float Theta = dot(viewDirectionWS, reflectWS);
	float Epsilon = CutOff - OuterCutOff;
	float Attenuation = clamp((Theta - OuterCutOff) / Epsilon, 0, 1);
	
	#ifdef BLINN_PHONG
		float3 HalfVector = normalize(viewDirectionWS + LightDirectionWS);
		float NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
		float specularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;
	#else
		float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));
		float specularFactor = pow(viewReflectionWS, SpecularPower) * NormalDotLightWS;
	#endif
	
	// Light aportation
	float3 Diffuse = DiffuseIntensity * DiffuseColor * NormalDotLightWS * Attenuation;
	float3 Ambient = AmbientIntensity * (1.0 - NormalDotLightWS) * AmbientColor * Attenuation;
	float3 Specular = SpecularIntensity * SpecularColor * specularFactor * Attenuation;

	output.Col.xyz = Diffuse.xyz + Ambient.xyz + Specular.xyz;

	#else
		float distanceWS = length(mul(LightPosition, View) - mul(mul(input.Pos, Model), View));
		float Attenuation = (10000.0 / (Constant + (Linear * distanceWS) + (Quadratic * (distanceWS * distanceWS))));

		float3 NormalWS = normalize(mul(float4(input.Nor.xyz, 0), Model)).xyz;
		float3 lightDirectionWS = -normalize(DirectionalLight.xyz);
		float NormalDotLightWS = max(0.0f, dot(lightDirectionWS, NormalWS.xyz));
		float3 viewDirectionWS = -normalize(LightDirection);
	
		#ifdef BLINN_PHONG
			float3 HalfVector = normalize(viewDirectionWS + lightDirectionWS);
			float NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
			float specularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;
		#else
			float3 reflectWS = normalize(reflect(-lightDirectionWS.xyz, NormalWS.xyz));
			float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));
			float specularFactor = pow(viewReflectionWS, SpecularPower) * NormalDotLightWS;
		#endif
	
		// Light aportation
		float3 Diffuse = DiffuseIntensity * DiffuseColor * NormalDotLightWS;
		float3 Specular = SpecularIntensity * SpecularColor * specularFactor;
		float3 Ambient = AmbientIntensity * (1.0 - NormalDotLightWS) * AmbientColor;
	
		output.Col.xyz = Ambient.xyz + Diffuse.xyz + Specular.xyz;
	#endif
#endif
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
#ifdef PIXEL_LIGHT
	#ifdef POINT_LIGHT

		//Pixel Normal
		float3 BinormalWS = normalize(cross(input.Nor, input.Tan));
		float3 TexNormal = txNormal.Sample(samLinear, input.Tex).xyz;
		TexNormal.xyz = normalize(2.0f * TexNormal.xyz - 1.0f);
		float3x3 TBNMatrix = float3x3(input.Tan, BinormalWS, input.Nor);
		float3 NormalWS = normalize(mul(TexNormal, TBNMatrix));
		
		// Lambert light calc
		float3 LightPositionWS = mul(mul(input.PosWS, Model), View);
		float3 LightDirectionWS = -normalize(LightPositionWS);
		float NormalDotLightWS = max(0.0f, dot(LightDirectionWS, input.Nor));
		float3 viewDirectionWS = -normalize(input.PosWS.xyz - LightDirection.xyz);
		
		float distanceWS = length(LightPosition - mul(input.PosWS, Model));
		float Attenuation = (1000.0 / (Constant + (Linear * distanceWS) + (Quadratic * (distanceWS * distanceWS))));
		
		#ifdef BLINN_PHONG
			float3 HalfVector = normalize(viewDirectionWS + LightDirectionWS);
			float NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
			float specularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;
		#else
			float3 reflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));
			float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));
			float specularFactor = pow(viewReflectionWS, SpecularPower) * NormalDotLightWS;
		#endif
		
		// Light aportation
		float3 Diffuse = DiffuseIntensity * DiffuseColor * NormalDotLightWS *Attenuation;
		float3 Ambient = AmbientIntensity * (1.0 - NormalDotLightWS) * AmbientColor * Attenuation;
		float3 Specular = SpecularIntensity * SpecularColor * specularFactor * Attenuation;

		return txDiffuse.Sample(samLinear, input.Tex) * float4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);

	#elif defined SPOT_LIGHT

	#else
		//Pixel Normal
		float3 BinormalWS = normalize(cross(input.Nor, input.Tan));
		float3 TexNormal = txNormal.Sample(samLinear, input.Tex).xyz;
		TexNormal.xyz = normalize(2.0f * TexNormal.xyz - 1.0f);
		float3x3 TBNMatrix = float3x3(input.Tan, BinormalWS, input.Nor);
		float3 NormalWS = normalize(mul(TexNormal, TBNMatrix));
	
		// Lambert light calc
		float3 LightDirectionWS = -normalize(DirectionalLight.xyz);
		float NormalDotLightWS = max(0.0f, dot(LightDirectionWS.xyz, input.Nor.xyz));
		float3 ViewDirectionWS = -normalize(input.PosWS.xyz - LightDirection.xyz);
	
		// Blinn specular
		#ifdef BLINN_PHONG
			float3 HalfVector = normalize(ViewDirectionWS.xyz + LightDirectionWS);
			float NormalDotHalfWS = max(0.0f, dot(HalfVector.xyz, NormalWS.xyz));
			float specularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;
		#else
			float3 ReflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));
			float ViewDotReflectionWS = max(0.0f, dot(ViewDirectionWS.xyz, ReflectWS.xyz));
			float specularFactor = pow(ViewDotReflectionWS, SpecularPower) * NormalDotLightWS;
		#endif
	
		float3 Diffuse = DiffuseIntensity * DiffuseColor * NormalDotLightWS;
		float3 Specular = SpecularIntensity * SpecularColor * specularFactor;
		float3 Ambient = AmbientIntensity * (1.0 - NormalDotLightWS) * AmbientColor;
	
		return txDiffuse.Sample(samLinear, input.Tex) * float4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);
	#endif
#else
	return txDiffuse.Sample(samLinear, input.Tex) * input.Col;
#endif

}
