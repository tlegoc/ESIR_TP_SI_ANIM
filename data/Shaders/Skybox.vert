#version 330 core

// La position d'un sommet du cube
layout (location = 0) in vec3 in_position;

// Coordonnées de texture
out vec3 var_textureCoordinates;

uniform mat4 uni_mat_projection;
uniform mat4 uni_mat_view;

void main()
{
	// position = direction = coordonnées de texture
	var_textureCoordinates = in_position;
	gl_Position = uni_mat_projection * uni_mat_view * vec4(in_position, 1.0);
}