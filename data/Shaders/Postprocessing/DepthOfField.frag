#version 330

in vec2 var_texcoord;

layout (location=0) out vec3 out_color;

uniform sampler2D uni_colorInput;
uniform sampler2D uni_depth;
uniform int uni_gaussianSize;


float g(float x, float y, float r)
{
	float top1 = x*x + y*y;
	float bottom1 = 2 * r * r;
	float top2 = exp(-top1/bottom1);
	float bottom2 = 2 * 3.1415926535897932384626 * r;

	return top2/bottom2;
}

void main()
{
	vec2 pixelSize = 1.0/textureSize(uni_colorInput, 0);

	float dist = texture(uni_depth, var_texcoord).r;
	float dist_center = texture(uni_depth, vec2(0.5, 0.5)).r;
	float ratio = clamp((dist-dist_center)/(1.0-dist_center), 0.0, 1.0);

	if (dist <= dist_center) {
		out_color = texture(uni_colorInput, var_texcoord).rgb;
		return;
	}

	vec3 o = vec3(0.0);

	float r = uni_gaussianSize + ratio * uni_gaussianSize;
	float totalG = 0.0;

	for (int i = -uni_gaussianSize; i <= uni_gaussianSize; i++)
	{
		for (int j = -uni_gaussianSize; j <= uni_gaussianSize; j++)
		{
			float g_v = g(i, j, r);
			totalG += g_v;

			vec2 offset = vec2(i * pixelSize.x, j * pixelSize.y);
			o +=  texture(uni_colorInput, var_texcoord + offset).rgb * g_v;
		}
	}

	out_color = o/totalG;
}