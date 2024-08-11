#version 330 core

out vec3 FragColor;

// Les coordonnées de texture
in vec3 var_textureCoordinates;

// La skybox i.e. la carte d'environnement
uniform samplerCube uni_skybox;

void main()
{
	FragColor = texture(uni_skybox, normalize(var_textureCoordinates)).rgb;
	// FragColor = vec3(1.0);
}