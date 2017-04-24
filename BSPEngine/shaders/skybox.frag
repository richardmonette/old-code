uniform sampler2D diffuseSampler;

void main()
{
	vec4 diffuse = texture2D(diffuseSampler, gl_TexCoord[0].st);

	gl_FragColor = diffuse;
}