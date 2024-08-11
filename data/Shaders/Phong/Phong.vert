uniform vec3 lightPosition ;

varying vec3 normalVector ;
varying vec3 vertexEye ; 

void main(void)
{
   vertexEye = (gl_ModelViewMatrix * gl_Vertex).xyz ;
   normalVector = gl_NormalMatrix * gl_Normal;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex ;
   gl_TexCoord[0] = gl_MultiTexCoord0 ;
}
