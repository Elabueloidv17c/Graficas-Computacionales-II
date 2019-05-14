#version 400 core
#define PIXEL_LIGHT
#define BLINN_PHONG

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;

uniform mat4 World;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

#ifndef PIXEL_LIGHT
	uniform vec4 DiffuseColor;
	uniform vec4 SpecularColor;
	uniform vec4 AmbientColor;

	uniform float DiffuseIntensity;
	uniform float SpecularIntensity;
	uniform float AmbientIntensity;

	uniform vec3 DirectionalLight;
	uniform float SpecularPower;
#endif

out vec2 TexCoord;

#ifdef PIXEL_LIGHT
	out vec3 Position;
	out vec3 Normal;
	out vec3 Tangent;
	out mat4 viewMatrix;
#else
	out vec4 Color;
	out vec4 Light;
#endif

void main()
{
    gl_Position = Projection * View * Model * vec4(inPosition, 1.0f);
	TexCoord = vec2(inTexCoord.x, inTexCoord.y);
	
#ifdef PIXEL_LIGHT
	Position = inPosition;

	vec4 normalWS = vec4((Model * vec4(inNormal, 0.0f)).xyzw);
	vec4 tangentWS = vec4((Model * vec4(inTangent, 0.0f)).xyzw);

	Tangent = tangentWS.xyz;
	Normal = normalWS.xyz;

	viewMatrix = View;
#else
	vec3 lightDirectionWS = normalize(-DirectionalLight.xyz);
	vec4 NormalWS = (Model * vec4(inNormal, 0.0f)).xyzw;
	float product = dot(lightDirectionWS, NormalWS.xyz);
	
	Light.xyzw = vec4(product, product, product, 1);

	vec3 cameraPos = vec3(View[0][3], View[1][3], View[2][3]);
	vec3 viewDirectionWS = -normalize(inPosition - cameraPos);

	vec3 reflectWS = normalize(reflect(-lightDirectionWS.xyz, NormalWS.xyz));
	float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));

	float specularFactor = pow(viewReflectionWS, SpecularPower) * product;

	vec3 finalColor = (Light.xyz * DiffuseColor.xyz) + (vec3(specularFactor, specularFactor, specularFactor) * SpecularColor.xyz);
	
	Color = vec4(finalColor.xyz, 1);
#endif
}