#version 330

// ----------
// The inputs
// ----------

// Vertex in local coordinate system
layout (location=0) in vec3 in_position;
// Normal to the vertex
layout (location=1) in vec3 in_normal;
// Texture coordinates associated with the vertex
layout (location=2) in vec2 in_textureCoordinates;

// -----------
// The outputs
// -----------

// Position in world coordinates
out vec3 var_position; 
// Normal in world coordinates
out vec3 var_normal;	
// Texture coordinates
out vec2 var_textureCoordinates;

// ------------
// The uniforms
// ------------

// Model to world transform matrix
uniform mat4 uni_mat_model;
// World to view transform matrix
uniform mat4 uni_mat_view;
// Projection matrix
uniform mat4 uni_mat_projection;
// Normal transformation matrix (model to world)
uniform mat3 uni_mat_normal;

void main()
{
	gl_Position =  uni_mat_projection * uni_mat_view * uni_mat_model * vec4(in_position, 1.0);
	var_position = vec3(uni_mat_model * vec4(in_position, 1.0));
	var_normal = normalize(uni_mat_normal * in_normal);
	var_textureCoordinates = in_textureCoordinates;
}