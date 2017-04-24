varying vec4 refrCoords; 
varying vec4 normCoords; 
varying vec4 viewCoords;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D normal;
uniform sampler2D dudv;

void main()
{
	const float kDistortion = 0.015;
	const float kRefraction = 0.009;

	vec4 distOffset = texture2D(dudv, normCoords.xy) * kDistortion;
	vec4 dudvColor = texture2D(dudv, vec2(refrCoords + distOffset));
	dudvColor = normalize(dudvColor * 2.0 - 1.0) * kRefraction;

	vec4 normalVector = texture2D(normal, vec2(refrCoords + distOffset));
	normalVector = normalVector * 2.0 - 1.0;
	normalVector.a = 0.0;

	vec4 projCoord = viewCoords / viewCoords.q;
	projCoord = (projCoord + 1.0) * 0.5;
	projCoord += dudvColor;
	projCoord = clamp(projCoord, 0.001, 0.999);
	
	vec4 reflectionColor  = texture2D(reflection, projCoord.xy);
	vec4 refractionColor  = texture2D(refraction, projCoord.xy);

	gl_FragColor = refractionColor * 0.5 + reflectionColor * 0.5; 
}