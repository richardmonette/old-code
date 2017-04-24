uniform sampler2D diffuseSampler;
uniform sampler2D lightmapSampler;

uniform int noLightmaps;
uniform int noTextures;

void main()
{
	vec4 diffuse = texture2D(diffuseSampler, gl_TexCoord[0].st);
	vec4 lightmap = texture2D(lightmapSampler, gl_TexCoord[1].st);

	if (diffuse.r > 0.99 && diffuse.g > 0.99 && diffuse.b > 0.99)
	{
		lightmap = vec4(1,1,1,1);
	}

	if (noLightmaps == 1)
	{
		lightmap = vec4(1,1,1,1);
	}

	if (noTextures == 1)
	{
		diffuse = vec4(1,1,1,1);
	}

	gl_FragColor = diffuse * lightmap;
}