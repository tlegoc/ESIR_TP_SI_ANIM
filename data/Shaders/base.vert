#version 330

in vec3 in_position;
in vec3 in_color;

out vec3 var_vertexColor;

uniform mat4 uni_mat_view;
uniform mat4 uni_mat_model;
uniform mat4 uni_mat_projection;

void main()
{
	var_vertexColor = in_color;
	gl_Position = uni_mat_projection * uni_mat_view * uni_mat_model * vec4(in_position, 1.0);
}