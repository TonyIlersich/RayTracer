R""(
#version 330 core

in vec2 uv;

out vec4 FragColor;

uniform sampler2D screenTexture;

// https://jo.dreggn.org/home/2010_atrous.pdf
const float atrousWeights[3] = float[3](3.0/8.0, 1.0/4.0, 1.0/16.0);
vec4 atrous(ivec2 center, int stepSize)
{
	ivec2 texSize = textureSize(screenTexture, 0);
	vec4 result = vec4(0);
	for (int dx = -2; dx <= 2; dx++)
	{
		for (int dy = -2; dy <= 2; dy++)
		{
			float weight = atrousWeights[max(abs(dx), abs(dy))];
			vec4 sample = texelFetch(
				screenTexture,
				clamp(center + ivec2(dx * stepSize, dy * stepSize), ivec2(0), texSize - 1),
				0);
			result += weight * sample;
		}
	}
	return max(result, texelFetch(screenTexture, center, 0));
}

void main()
{
	ivec2 texSize = textureSize(screenTexture, 0);
	ivec2 texCoord = ivec2(uv * vec2(texSize));
	FragColor = atrous(texCoord, 1);
}
)""