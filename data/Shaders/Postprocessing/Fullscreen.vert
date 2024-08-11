#version 330

out vec2 var_texcoord;

void main()
{
	vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
	vec2 texCoord = (position + 1) * 0.5;

	gl_Position = vec4(position, 0.0, 1.0);
	var_texcoord = texCoord;
}