layout(location = 0) out vec4 color;

in vec2 TexCoord;

uniform sampler2D LightingPass;
uniform sampler2D LuminancePass;

uniform float LevelOfDetail;
uniform float BloomThreshold;

void main()
{
	vec4 col = texture2D(LightingPass, TexCoord.xy);

	float bright = textureLod(LuminancePass, vec2(TexCoord.xy), LevelOfDetail).x;
	bright = max(bright - BloomThreshold, 0.0f);

	color = col*vec4(bright, bright, bright, bright);
}