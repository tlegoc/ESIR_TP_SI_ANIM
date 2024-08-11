// Defines a Phong Shader using a unique texture (for diffuse and specular)
// This shader handles several lights that can be parameterized using macro definitions

// Undefine to produce a kind of toon shader
//#define useQuatifier 5

// Define this to use texture 0 for lighting
//#define USE_TEXTURE0

// Define one or several of theses to enable lights.
//#define USE_LIGHT0
//#define USE_LIGHT1
//#define USE_LIGHT2
//#define USE_LIGHT3
//#define USE_LIGHT4
//#define USE_LIGHT5
//#define USE_LIGHT6
//#define USE_LIGHT7

uniform sampler2D Texture0 ; 

//uniform vec4 materialAmbient ;
//uniform vec4 materialDiffuse ;
//uniform vec4 materialSpecular ;
//uniform float materialShininess ;

//uniform vec3 lightPosition ;
//uniform vec4 lightAmbient ;
//uniform vec4 lightDiffuse ;
//uniform vec4 lightSpecular ;

varying vec3 normalVector ;
varying vec3 vertexEye ;

float quantify(float value, float steps)
{
   return floor(value*steps)/steps ;
}

// Computes the diffuse factor given normalized light direction and surface normal 
float diffuseFactor(vec3 lightDirectionNormalized, vec3 normalNormalized) 
{
   float factor = max(dot(lightDirectionNormalized, normalNormalized), 0.0) ;
   #ifdef useQuatifier
   return quantify(factor,useQuatifier) ;
   #else
   return factor ;
   #endif
}

// Computes the specular factor given normalized light direction, surface normal and eye direction and shininess
float specularFactor(vec3 lightDirectionNormalized, vec3 normalNormalized, vec3 eyeDirectionNormalized, float shininess) 
{
   vec3 reflected = -reflect(lightDirectionNormalized, normalNormalized) ;
   float factor = pow(max(dot(eyeDirectionNormalized, reflected),0.0), max(shininess,1.0)) ;
   #ifdef useQuantifier
   return quantify(factor, useQuatifier) ;
   #else
   return factor ;
   #endif
}

vec4 computeLighting(int lightNumber, vec3 normalVectorNormalized, vec3 eyeDirectionNormalized)
{
   vec3 lightDirectionNormalized = normalize(gl_LightSource[lightNumber].position.xyz-vertexEye) ;
   float diffuse = diffuseFactor(lightDirectionNormalized, normalVectorNormalized) ; ;
   float specular = specularFactor(lightDirectionNormalized, normalVectorNormalized, eyeDirectionNormalized, gl_FrontMaterial.shininess) ;

   return gl_FrontMaterial.diffuse*gl_LightSource[lightNumber].diffuse*diffuse       // Diffuse
        + gl_FrontMaterial.specular*gl_LightSource[lightNumber].specular*specular    // Specular
        + gl_FrontMaterial.ambient*gl_LightSource[lightNumber].ambient               // Ambient
        //+ gl_FrontMaterial.emissive ;
		;
}

void main(void)
{
   //vec3 lightDirectionNormalized = normalize(lightPosition-vertexEye) ; 
   vec3 normalVectorNormalized = normalize(normalVector) ;
   vec3 eyeDirectionNormalized = normalize(-vertexEye) ;
   
   // Lighting computation 
   vec4 lighting = vec4(0.0,0.0,0.0,0.0) ;
   #ifdef USE_LIGHT0
   lighting += computeLighting(0, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   #ifdef USE_LIGHT1
   lighting += computeLighting(1, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   #ifdef USE_LIGHT2
   lighting += computeLighting(2, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   #ifdef USE_LIGHT3
   lighting += computeLighting(3, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   #ifdef USE_LIGHT4
   lighting += computeLighting(4, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   #ifdef USE_LIGHT5
   lighting += computeLighting(5, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   #ifdef USE_LIGHT6
   lighting += computeLighting(6, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   #ifdef USE_LIGHT7
   lighting += computeLighting(7, normalVectorNormalized, eyeDirectionNormalized) ;
   #endif
   
   #ifdef USE_TEXTURE0
   gl_FragColor = texture2D(Texture0, gl_TexCoord[0].st)+lighting ;
   #else
   gl_FragColor = lighting ;
   #endif
     
   // Simple tone mapper
   //vec4 color = texture2D(Texture0, gl_TexCoord[0].st)*(lightDiffuse*diffuse+lightSpecular*specular+lightAmbient);
   //vec4 toneMapped = color/(color + vec4(1.0,1.0,1.0,1.0)) ;
   //gl_FragColor = toneMapped; 
}