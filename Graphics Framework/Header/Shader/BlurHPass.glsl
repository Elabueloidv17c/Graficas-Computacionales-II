layout(location = 0) out vec4 color;

in vec2 TexCoord;

uniform sampler2D BrightPass;

uniform vec2 ViewportDimensions;
uniform int LevelOfDetail;

void main()
{
	vec2 OFFSETS0 = vec2(-4, 0);
	vec2 OFFSETS1 = vec2(-3, 0);
	vec2 OFFSETS2 = vec2(-2, 0);
	vec2 OFFSETS3 = vec2(-1, 0);
	vec2 OFFSETS4 = vec2(0, 0);
	vec2 OFFSETS5 = vec2(1, 0);
	vec2 OFFSETS6 = vec2(2, 0);
	vec2 OFFSETS7 = vec2(3, 0);
	vec2 OFFSETS8 = vec2(4, 0);

	//Gauss  Distribution
	float BlurWeights0 = 0.004815026f; 
	float BlurWeights1 = 0.028716039f; 
	float BlurWeights2 = 0.102818575f; 
	float BlurWeights3 = 0.221024189f; 
	float BlurWeights4 = 0.285252340f; 
	float BlurWeights5 = 0.221024189f; 
	float BlurWeights6 = 0.102818575f; 
	float BlurWeights7 = 0.028716039f; 
	float BlurWeights8 = 0.004815026f; 

	float Texel = (1.0f / ViewportDimensions.x) * (LevelOfDetail + 1);

	vec4
	output  = textureLod(BrightPass, vec2(TexCoord + (OFFSETS0 * Texel)), LevelOfDetail) * BlurWeights0;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS1 * Texel)), LevelOfDetail) * BlurWeights1;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS2 * Texel)), LevelOfDetail) * BlurWeights2;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS3 * Texel)), LevelOfDetail) * BlurWeights3;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS4 * Texel)), LevelOfDetail) * BlurWeights4;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS5 * Texel)), LevelOfDetail) * BlurWeights5;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS6 * Texel)), LevelOfDetail) * BlurWeights6;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS7 * Texel)), LevelOfDetail) * BlurWeights7;
	output += textureLod(BrightPass, vec2(TexCoord + (OFFSETS8 * Texel)), LevelOfDetail) * BlurWeights8;

	color = output;
}