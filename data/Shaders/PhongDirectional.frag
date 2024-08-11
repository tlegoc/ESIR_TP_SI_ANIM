#version 330

// -----------
// The outputs
// -----------
layout (location=0) out vec3 out_color;

// ----------
// The inputs
// ----------

// Position in view coordinates
in vec3 var_position; 
// Normal in view coordinates
in vec3 var_normal;	
// Texture coordinates
in vec2 var_textureCoordinates;

// ------------
// The uniforms
// ------------

// The diffuse texture
uniform sampler2D uni_diffuseTexture;
// The specular texture
uniform sampler2D uni_specularTexture;
// The viewer position (in world space)
uniform vec3 uni_viewerPosition;

uniform samplerCube uni_skybox;

// fog
uniform float uni_far_clipping;

// The light (world coordinates)
uniform struct DirectionalLight
{
	// Light position in world coordinates
	vec3 direction;
	// The light color
	vec3 color;
} uni_light;

// The material
uniform struct Material
{
	// The diffuse color
	vec3 diffuseColor;
	// The specular color
	vec3 specularColor;
	// The shininess
	float shininess;
} uni_material;

// Computes the diffuse component
vec3 diffuse(vec3 lightDirection, vec3 lightColor, vec3 point, vec3 normal, vec3 pointColor)
{
	float cosine = max(dot(lightDirection,normal), 0.0);
	return lightColor * pointColor * cosine;
}

// Computes the specular component
vec3 specular(vec3 lightDirection, vec3 lightColor, vec3 point, vec3 normal, vec3 pointColor, float shininess)
{
	float illuminatedFront = (dot(lightDirection, normal)>0)?1.0:0.0;
	vec3 reflected = reflect(-lightDirection, normal);
	vec3 viewerDirection = normalize(uni_viewerPosition-point); 

	vec3 reflectionDir = normalize(reflect(-viewerDirection, normal));
	vec3 reflectionColor = texture(uni_skybox, reflectionDir).rgb;

	float cosine = max(dot(reflected, viewerDirection), 0.0);

	return illuminatedFront * lightColor * pointColor * pow(cosine, shininess) + pointColor * reflectionColor;
}

void main()
{
	vec3 normal = normalize(var_normal);

	vec3 diffuseColor = uni_material.diffuseColor*texture(uni_diffuseTexture, var_textureCoordinates.st).rgb;
	vec3 specularColor = uni_material.specularColor*texture(uni_diffuseTexture, var_textureCoordinates.st).rgb*texture(uni_specularTexture, var_textureCoordinates).rgb;
	vec3 lightDirection = -uni_light.direction;

	vec3 fragColor = diffuse(lightDirection, uni_light.color, var_position, normalize(var_normal), diffuseColor)  
			    + specular(lightDirection, uni_light.color, var_position, normalize(var_normal), specularColor, uni_material.shininess) ;


	float fogFactor = length(var_position - uni_viewerPosition);
	
	vec3 viewerDirection = -normalize(uni_viewerPosition-var_position); 
	vec3 skybox = texture(uni_skybox, viewerDirection).rgb;

	out_color = mix(fragColor, skybox, pow(clamp(fogFactor/uni_far_clipping, 0, 1), 4));
}