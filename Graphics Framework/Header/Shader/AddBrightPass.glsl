layout(location = 0) out vec4 color;

in vec2 TexCoord;

uniform sampler2D BrightPass;
uniform sampler2D BlurHPass;
uniform sampler2D BlurVPass;

uniform int LevelOfDetail;

void main()
{
	vec4 Bright = texture2D(BrightPass, TexCoord);
	vec4 BlurH = textureLod(BlurHPass, TexCoord, LevelOfDetail + 1);
	vec4 BlurV = textureLod(BlurVPass, TexCoord, LevelOfDetail + 1);

	color = (0.5f * (BlurH + BlurV) * Bright);
}