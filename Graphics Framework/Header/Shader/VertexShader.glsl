#version 400 core

#define PIXEL_LIGHT

#define DIRECTIONAL_LIGHT
#define POINT_LIGHT
#define SPOT_LIGHT

#define BLINN_PHONG

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;

uniform mat4 World;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

#ifdef PIXEL_LIGHT
	out vec3 inPosWS;
	out vec3 Normal;
	out vec3 Tangent;
#else
	uniform vec4	DiffuseColor;
	uniform vec4	SpecularColor;
	uniform vec4	AmbientColor;
	
	uniform float	DiffuseIntensity;
	uniform float	SpecularIntensity;
	uniform float	AmbientIntensity;
	
	uniform vec3	DirectionalLight;
	uniform float	SpecularPower;
	
	uniform vec3	PointPosition;
	uniform float   PointRadius;
	
	uniform vec3	ViewPosition;
	uniform float   SpotRadius;
	
	uniform vec3	SpotPosition;
	uniform float   SpotAlpha;
	
	uniform vec3	SpotLightDirection;
	uniform float   SpotBeta;
	
	out vec4 Color;
#endif

out vec2 TexCoord;

void main()
{
	gl_Position = Projection * View * Model * vec4(inPosition, 1.0f);
	TexCoord = inTexCoord;

#ifdef PIXEL_LIGHT
	inPosWS.xyz = (Model * vec4(inPosition, 1.0f)).xyz;
	Normal.xyz = (Model * vec4(inNormal, 1.0f)).xyz;
	Tangent.xyz = (Model * vec4(inTangent, 1.0f)).xyz;
#else
	vec4 NormalWS = (Model * vec4(inNormal, 0.0f)).xyzw;
	vec3 PosWS = (Model * vec4(inPosition, 0.0f)).xyz;

	vec3 ViewDirectionWS;
	vec3 LightDirectionWS;
	float NormalDotLightWS;
	float SpecularFactor;

	vec3 PointViewDirectionWS;
	vec3 PointLightDirectionWS;
	float PointNormalDotLightWS;
	float PointSpecularFactor;


	vec3 SpotViewDirectionWS;
	vec3 SpotLightDirectionWS;
	float SpotNormalDotLightWS;
	float SpotSpecularFactor;

	float Distance;
	//float LightNumber = 1.0f + max(0.0f, (SpotRadius / SpotRadius)) + max(0.0f, (PointRadius / PointRadius));

#ifdef DIRECTIONAL_LIGHT
	LightDirectionWS = -normalize(DirectionalLight.xyz);
	ViewDirectionWS = -normalize(DirectionalLight.xyz);
	NormalDotLightWS = max(0.0f, dot(LightDirectionWS, NormalWS.xyz));
#endif
#ifdef POINT_LIGHT
	PointLightDirectionWS = -normalize(PosWS.xyz - (Model * vec4(PointPosition.xyz, 1.0f)).xyz);
	PointViewDirectionWS = PointLightDirectionWS;
	Distance = length(PosWS.xyz - (Model * vec4(PointPosition.xyz, 1.0f)).xyz);
	PointNormalDotLightWS = max(0.0f, dot(PointViewDirectionWS.xyz, NormalWS.xyz)) * (PointRadius / Distance);
#endif
#ifdef SPOT_LIGHT
	SpotViewDirectionWS = -normalize(PosWS - ViewPosition);
	SpotLightDirectionWS = -normalize(SpotLightDirection.xyz);

	vec3 SpotDirToVertex = -normalize(PosWS.xyz - ViewPosition.xyz);
	float Theta = dot(SpotDirToVertex, SpotLightDirectionWS.xyz);
	float Spot = Theta - cos(SpotBeta * 0.5);
	Spot = max(0.0, Spot / (cos(SpotAlpha * 0.5) - cos(SpotBeta * 0.5)));

	Distance = length(PosWS.xyz - ViewPosition.xyz);
	SpotNormalDotLightWS = max(0.0, dot(SpotLightDirectionWS, NormalWS.xyz) * Spot) * (SpotRadius / Distance);
#endif
#ifdef BLINN_PHONG
	vec3 HalfVector = normalize(ViewDirectionWS.xyz + LightDirectionWS.xyz);
	float NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
	SpecularFactor = pow(NormalDotHalfWS, SpecularPower) * NormalDotLightWS;

	HalfVector = normalize(PointViewDirectionWS.xyz + PointLightDirectionWS.xyz);
	NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
	PointSpecularFactor = pow(NormalDotHalfWS, SpecularPower) * PointNormalDotLightWS;

	HalfVector = normalize(SpotViewDirectionWS.xyz + SpotLightDirectionWS.xyz);
	NormalDotHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
	SpotSpecularFactor = pow(NormalDotHalfWS, SpecularPower) * SpotNormalDotLightWS;
#else
	vec3 ReflectWS = normalize(reflect(-LightDirectionWS.xyz, NormalWS.xyz));
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
	vec3 Diffuse = DiffuseColor.xyz * DiffuseIntensity * NormalDotLightWS;
	vec3 Specular = SpecularColor.xyz * SpecularIntensity * SpecularFactor;
	float Ambient = AmbientIntensity * (1.0 - NormalDotLightWS);

	#ifdef POINT_LIGHT
		Diffuse += DiffuseColor.xyz * DiffuseIntensity * PointNormalDotLightWS;
		Specular += SpecularColor.xyz * SpecularIntensity * PointSpecularFactor;
		Ambient *= (1.0 - PointNormalDotLightWS);
		//Diffuse /= LightNumber.xxx;
	#endif
	
	#ifdef SPOT_LIGHT
		Diffuse += DiffuseColor.xyz * DiffuseIntensity * SpotNormalDotLightWS;
		Specular += SpecularColor.xyz * SpecularIntensity * SpotSpecularFactor;
		Ambient *= (1.0 - SpotNormalDotLightWS);
		//Specular /= LightNumber.xxx;
	#endif

	Color = vec4(vec3(Ambient, Ambient, Ambient) + Diffuse.xyz + Specular.xyz, 1.0);
#endif
}