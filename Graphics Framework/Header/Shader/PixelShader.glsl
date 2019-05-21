#version 400 core

#define PIXEL_LIGHT

#define DIRECTIONAL_LIGHT
#define POINT_LIGHT
#define SPOT_LIGHT

#define BLINN_PHONG

in vec2 TexCoord;

#ifdef PIXEL_LIGHT
	in vec3 inPosWS;
	in vec3 Normal;
	in vec3 Tangent;

	uniform mat4	Model;

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
#else
	in vec4 Color;
#endif

out vec4 FinalColor;

uniform sampler2D Texture;
uniform sampler2D NormalMap;

void main()
{	
#ifdef PIXEL_LIGHT
	//Pixel Normal
	vec3 BinormalWS = normalize(cross(Normal.xyz, Tangent.xyz));
	vec3 TexNormal = texture(NormalMap, TexCoord).xyz;
	TexNormal.xyz = normalize(2.0f * TexNormal.xyz - 1.0f);
	mat3 TBNMatrix = mat3(Tangent, BinormalWS, Normal);
	vec3 NormalWS = normalize(TBNMatrix * TexNormal).xyz;

	vec3 PosWS = inPosWS.xyz;

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

	FinalColor = texture(Texture, TexCoord) * vec4(vec3(Ambient, Ambient, Ambient) + Diffuse.xyz + Specular.xyz, 1.0);
#else
	FinalColor = texture(Texture, TexCoord) * Color;
#endif
}