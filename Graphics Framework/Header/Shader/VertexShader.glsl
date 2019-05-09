#version 400 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 World;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	Position = inPosition;
    gl_Position = Projection * View * Model * vec4(inPosition, 1.0f);

	TexCoord = vec2(inTexCoord.x, inTexCoord.y);
	
	vec4 normalWS = vec4((Model * vec4(inNormal, 0.0f)).xyzw);
	
	Normal = normalWS.xyz;
}