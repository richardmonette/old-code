varying vec4 refrCoords; 
varying vec4 normCoords; 
varying vec4 viewCoords;

uniform float time;

void main()
{
	refrCoords = gl_MultiTexCoord1;
	normCoords = gl_MultiTexCoord2;

	vec4 warped = gl_Vertex;
	warped.y += sin(time + (warped.z/7.0)) * cos(time + (warped.x/8.0));

	viewCoords = gl_ModelViewProjectionMatrix * warped;

	gl_Position = gl_ModelViewProjectionMatrix * warped;
}