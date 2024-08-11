#version 330

in vec3 var_position;
in vec3 var_normal;
in vec3 var_tangent;
in vec3 var_bitangent;
in vec2 var_texcoord;

uniform mat4 uni_mat_view;
uniform mat4 uni_mat_model;
uniform mat4 uni_mat_projection;

// Material parameters
uniform vec3 uni_diffuseColor;
// uniform vec3 uni_specularColor;
uniform float uni_shininess;
uniform vec3 uni_camPos;

// Textures
uniform sampler2D uni_texDiff;
uniform sampler2D uni_texSpec;
uniform sampler2D uni_texNorm;

uniform int uni_lightCount;
uniform vec3 uni_lightPositions[16];
uniform vec3 uni_lightColors[16];

out vec3 out_fragColor;

vec3 phong()
{

	// Calcul de la normale avec texture
	// vec3 N = normalize(var_normal);
	mat3 TBN = mat3(normalize(var_tangent), normalize(var_bitangent), normalize(var_normal));
	vec3 normalTex = (texture(uni_texNorm, var_texcoord).rgb - vec3(.5, .5, .5))*2.0;
	vec3 N = normalize(TBN * normalTex);

	// position to camera, view vector
	vec3 V = normalize(uni_camPos - var_position);

	
	vec3 Ks = texture(uni_texSpec, var_texcoord).rgb;
	vec3 Kd = vec3(1.0) - Ks;
	vec3 diffuseColor = uni_diffuseColor * texture(uni_texDiff, var_texcoord).rgb;

	vec3 spec = vec3(0.0);
	vec3 diff =  vec3(0.0);
	for (int i = 0; i < uni_lightCount; i++) {
		// position to light
		vec3 lightDir = normalize(uni_lightPositions[i] - var_position);
		vec3 R = reflect(-lightDir, N);

		if (dot(lightDir, N) > 0) {
			spec += pow(max(dot(V, R), 0.0), uni_shininess) * uni_lightColors[i] * Ks;
		}
		
		diff += max(dot(lightDir, N), 0.0) * uni_lightColors[i] * diffuseColor * Kd;
	}

	vec3 x = diff + spec;
	return x/(x+vec3(1.0));
	// return texture(uni_texSpec, var_texcoord).rgb;
}

void main()
{
	out_fragColor = phong();
}