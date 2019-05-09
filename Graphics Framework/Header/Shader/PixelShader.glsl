#version 400 core

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform vec4 MeshColor;
uniform vec4 SpecularColor;
uniform vec3 DirectionalLight;
uniform float SpecularPower;

uniform mat4 View;

out vec4 FinalColor;

uniform sampler2D Texture;

void main()
{	
	vec3 lightDirectionWS = normalize(-DirectionalLight.xyz);
	float product = dot(lightDirectionWS, Normal);
	
	vec4 Light = vec4(product, product, product, 1);

	vec3 cameraPos = vec3(View[0][3], View[1][3], View[2][3]);

	vec3 viewDirectionWS = -normalize(Position - cameraPos);

	vec3 reflectWS = normalize(reflect(-lightDirectionWS.xyz, Normal.xyz));
	float viewReflectionWS = max(0.0f, dot(viewDirectionWS, reflectWS));

	float specularFactor = pow(viewReflectionWS, SpecularPower) * product;

	vec3 color = (Light.xyz * MeshColor.xyz) + (vec3(specularFactor, specularFactor, specularFactor) * SpecularColor.xyz);
	
	vec4 finalColor = vec4(color.xyz, 1);

	FinalColor = texture(Texture, TexCoord) * finalColor;
}