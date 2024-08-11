#version 330

in vec3 in_position;
in vec3 in_normal;
in vec3 in_tangent;
in vec3 in_bitangent;
in vec2 in_texcoord;

out vec3 var_position;
out vec3 var_normal;
out vec3 var_tangent;
out vec3 var_bitangent;
out vec2 var_texcoord;

uniform mat4 uni_mat_view;
uniform mat4 uni_mat_model;
uniform mat4 uni_mat_projection;
uniform mat3 uni_mat_normal;

void main()
{
	gl_Position = uni_mat_projection * uni_mat_view * uni_mat_model * vec4(in_position, 1.0);

	var_position = (uni_mat_model * vec4(in_position, 1.0)).xyz;
	var_normal = uni_mat_normal * in_normal;
	var_tangent = uni_mat_normal * in_tangent;
	var_bitangent = uni_mat_normal * in_bitangent;
	var_texcoord = in_texcoord;
}