#version 400 core

in vec2 TexCoord;
in vec4 Color;
in vec4 Light;

out vec4 FinalColor;

uniform sampler2D Texture;

void main()
{	
	FinalColor = texture(Texture, TexCoord) * Color * Light;
}