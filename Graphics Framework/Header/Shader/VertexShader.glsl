#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

out vec2 TexCoord;
out vec4 MeshColor;
out vec4 Light;

uniform vec4 color;
uniform vec3 light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    TexCoord = vec2(inTexCoord.x, inTexCoord.y);
	MeshColor = color;

	vec3 lightDirectionWS = normalize(-light);
	float product = dot(lightDirectionWS, inNormal);
	
	Light.xyzw = vec4(product, product, product, 1);
}