layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(inPosition.xyz, 1.0f);
	TexCoord = vec2(1.0f - inTexCoord.x, inTexCoord.y);
}