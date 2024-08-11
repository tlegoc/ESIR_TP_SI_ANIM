#version 330

in vec3 var_vertexColor;

out vec3 out_fragcolor;

void main()
{
	out_fragcolor = var_vertexColor;
}