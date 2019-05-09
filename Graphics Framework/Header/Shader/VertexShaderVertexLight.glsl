#version 400 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

out vec2 TexCoord;
out vec4 Color;
out vec4 Light;

uniform vec4 MeshColor;
uniform vec4 SpecularColor;
uniform vec3 DirectionalLight;
uniform float SpecularPower;

uniform mat4 World;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * View * Model * vec4(inPosition, 1.0f);
    TexCoord = vec2(inTexCoord.x, inTexCoord.y);
	
	vec3 lightDirectionWS = normalize(-DirectionalLight.xyz);
	vec4 NormalWS = (Model * vec4(inNormal, 0.0f)).xyzw;
	float product = dot(lightDirectionWS, NormalWS.xyz);
	
	Light.xyzw = vec4(product, product, product, 1);

	vec3 cameraPos = vec3(View[0][3], View[1][3], View[2][3]);
	vec3 viewDirectionWS = -normalize(inPosition - cameraPos);

	vec3 reflectWS = normalize(reflect(-lightDirectionWS.xyz, NormalWS.xyz));
	float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));

	float specularFactor = pow(viewReflectionWS, SpecularPower) * product;

	vec3 finalColor = (Light.xyz * MeshColor.xyz) + (vec3(specularFactor, specularFactor, specularFactor) * SpecularColor.xyz);
	
	Color = vec4(finalColor.xyz, 1);
}