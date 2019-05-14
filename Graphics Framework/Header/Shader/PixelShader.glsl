#version 400 core
#define PIXEL_LIGHT
#define BLINN_PHONG

in vec2 TexCoord;

#ifdef PIXEL_LIGHT
	in vec3 Position;
	in vec3 Normal;
	in vec3 Tangent;
	in mat4 viewMatrix;

	uniform vec4 DiffuseColor;
	uniform vec4 SpecularColor;	
	uniform vec4 AmbientColor;

	uniform float DiffuseIntensity;
	uniform float SpecularIntensity;
	uniform float AmbientIntensity;

	uniform vec3 DirectionalLight;
	uniform float SpecularPower;

	uniform mat4 View;
#else
	in vec4 Color;
	in vec4 Light;
#endif

out vec4 FinalColor;

uniform sampler2D Texture;
uniform sampler2D NormalMap;

void main()
{	
#ifdef PIXEL_LIGHT

	//Pixel Normal
	vec3 BinormalWS = normalize(cross(Normal, Tangent));
	
	vec4 TexNormal = texture(NormalMap, TexCoord);
	TexNormal = normalize(2.0f * TexNormal - 1.0f);
	
	mat3 TBNMatrix = mat3(Tangent, BinormalWS, Normal);
	vec3 NormalWS = normalize(TexNormal.xyz * TBNMatrix);
	
	// Lambert light calc
	vec3 LightDirectionWS = -normalize(DirectionalLight.xyz);
	float NormaldLightWS = max(0.0f, dot(LightDirectionWS.xyz, Normal.xyz));
	vec4 Light = vec4(NormaldLightWS, NormaldLightWS, NormaldLightWS, 1);

	// Blinn specular
	vec3 cameraPos = vec3(viewMatrix[0].w, viewMatrix[1].w, viewMatrix[2].w);
	vec3 ViewDirectionWS = -normalize(Position.xyz - cameraPos.xyz);

	float specularFactor;

	#ifdef BLINN_PHONG
		vec3 HalfVector = normalize(ViewDirectionWS + LightDirectionWS);
		float NormaldHalfWS = max(0.0f, dot(NormalWS.xyz, HalfVector.xyz));
		specularFactor = pow(NormaldHalfWS, SpecularPower) * NormaldLightWS;
	#else
		vec3 reflectWS = normalize(reflect(-LightDirectionWS, NormalWS));
		float viewReflectionWS = max(0.0f, dot(ViewDirectionWS, reflectWS));
		specularFactor = pow(viewReflectionWS, SpecularPower) * NormaldLightWS;
	#endif

	vec3 Diffuse = DiffuseIntensity * NormaldLightWS  * DiffuseColor.xyz;
	vec3 Specular = SpecularIntensity * specularFactor * SpecularColor.xyz;
	vec3 Ambient = AmbientIntensity * (1.0 - NormaldLightWS) * AmbientColor.xyz;

	//FinalColor = texture(Texture, TexCoord) * vec4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);
	FinalColor = vec4(Ambient.xyz + Diffuse.xyz + Specular.xyz, 1.0);
#else
	FinalColor = texture(Texture, TexCoord) * Color * Light;
#endif
}