layout(location = 0) out vec4 color;

in vec2 TexCoord;

uniform sampler2D AddBrightPass;
uniform sampler2D LightingPass;

uniform float Exposure;
uniform float BloomMultiplier;

vec3 BasicExposure(in vec3 Color)
{
	Color *= Exposure;
	Color = pow(Color, vec3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	return Color;
}

vec3 Reinhard(in vec3 Color)
{
	Color *= Exposure;
	Color /= (1.0f + Color);
	Color = pow(Color, vec3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	return Color;
}

vec3 Burgess_Dawson(in vec3 Color)
{
	Color *= Exposure;
	vec3 x = Color - 0.004f;
	Color = (x * (6.2 * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

	return Color;
}

vec3 Uncharted2Tonemap(in vec3 x)
{
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;

	vec3 Value;
	Value.x = ((x.x * (A * x.x + C * B) + D * E) / (x.x * (A * x.x + B) + D * F)) - E / F;
	Value.y = ((x.y * (A * x.y + C * B) + D * E) / (x.y * (A * x.y + B) + D * F)) - E / F;
	Value.z = ((x.z * (A * x.z + C * B) + D * E) / (x.z * (A * x.z + B) + D * F)) - E / F;
	return Value;
}

vec3 Uncharted2(in vec3 Color)
{
	Color.x *= Exposure;
	Color.y *= Exposure;
	Color.z *= Exposure;

	vec3 W = vec3(11.2f, 11.2f, 11.2f);
	vec3 ExposureBias = vec3(2.0f, 2.0f, 2.0f);
	
	vec3 curr = Uncharted2Tonemap(ExposureBias * Color);
	vec3 whiteScale = vec3(1.0f, 1.0f, 1.0f) / Uncharted2Tonemap(W);
	
	Color = curr * whiteScale;
	
	Color = pow(Color, vec3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
	return Color;
}

void main()
{
	// Take color 
	vec3 Color = texture2D(LightingPass, TexCoord.xy).xyz;
	
	// Add HDR
	Color += BloomMultiplier * texture2D(AddBrightPass, TexCoord.xy).xyz;
	color = vec4(Uncharted2(Color), 1.0f);

	//// Tatransform linear color to standard RGBA
	//if (TexCoord.x <= 0.5f && TexCoord.y > 0.5f)
	//{
	//	color = vec4(BasicExposure(Color), 1.0f);
	//}
	//else if (TexCoord.x <= 0.5f && TexCoord.y <= 0.5f)
	//{
	//	color = vec4(Uncharted2(Color), 1.0f);
	//}
	//else if (TexCoord.x > 0.5f && TexCoord.y > 0.5f)
	//{
	//	color = vec4(Burgess_Dawson(Color), 1.0f);
	//}
	//else if (TexCoord.x > 0.5f && TexCoord.y <= 0.5f)
	//{
	//	color = vec4(Reinhard(Color), 1.0f);
	//}
}