#version 330

in vec2 var_texcoord;

layout (location=0) out vec3 out_color;

uniform sampler2D uni_colorInput;
uniform float uni_p;
uniform int uni_l;

void main()
{
	vec3 inputColor = texture(uni_colorInput, var_texcoord).rgb;
	float n = length(inputColor)/sqrt(3.0);
	float f = pow((round(pow(n, uni_p) * uni_l)/uni_l), 1.0/uni_p);

	out_color = (inputColor/n)*f;
}