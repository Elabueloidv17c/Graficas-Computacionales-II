layout(location = 0) out vec3 color;
in vec2 TexCoord;

uniform sampler2D LightingPass;

void main() 
{
	//Luminance Factor
	vec4 LuminanceFactor = vec4(0.2f, 0.587f, 0.15f, 1.0f);

	//Texture sample
	vec4 col = texture2D(LightingPass, TexCoord.xy);

	//Luminance result
	float Luminance = dot(col, LuminanceFactor);
	Luminance = log(Luminance + 0.000001f);

	color = vec3(Luminance, Luminance, Luminance);
}