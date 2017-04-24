-- Vertex.GL3

in vec4 Position;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 DiffuseMaterial;

out vec4 pos;

void main()
{
    gl_Position = Modelview * Position;
    pos = Position;//Modelview * Position;
}

-- Geometry.GL3

#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable 

uniform mat4 ProjectionGeom;

out vec4 eye_position;
out vec2 TexCoord0;
out vec4 pos;

const float sphere_radius = 0.5;

void main()
{
    float halfsize = sphere_radius * 0.5;

	eye_position = gl_PositionIn[0];
	pos = gl_PositionIn[0];
	
    // Vertex 1
    TexCoord0.st = vec2(-1.0,-1.0);
    gl_Position = gl_PositionIn[0];
    gl_Position.xy += vec2(-halfsize, -halfsize);
    gl_Position = ProjectionGeom  * gl_Position;
    EmitVertex();

    // Vertex 2
    TexCoord0.st = vec2(-1.0,1.0);
    gl_Position = gl_PositionIn[0];
    gl_Position.xy += vec2(-halfsize, halfsize);
    gl_Position = ProjectionGeom  * gl_Position;
    EmitVertex();

    // Vertex 3
    TexCoord0.st = vec2(1.0,-1.0);
    gl_Position = gl_PositionIn[0];
    gl_Position.xy += vec2(halfsize, -halfsize);
    gl_Position = ProjectionGeom  * gl_Position;
    EmitVertex();

    // Vertex 4
    TexCoord0.st = vec2(1.0,1.0);
    gl_Position = gl_PositionIn[0];
    gl_Position.xy += vec2(halfsize, halfsize);
    gl_Position = ProjectionGeom  * gl_Position;
    EmitVertex();

    EndPrimitive();
}

-- Fragment.GL3

uniform sampler3D Density;

//http://morgan.leborgne.free.fr/wiki/index.php5?title=GLSL_Shaders#Generate_billboard_using_a_GLSL_Geometry_Shader

in vec4 pos;
in vec2 TexCoord0;
in vec4 eye_position;

out vec4 FragColor;

uniform vec3 LightPosition;
uniform vec3 AmbientMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;

const float sphere_radius = 0.5;

uniform vec3 LightIntensity = vec3(1.0);
uniform float Absorption = 10.0;

const float maxDist = sqrt(2.0);
const int numSamples = 128;
const float stepSize = maxDist / float(numSamples);
const int numLightSamples = 32;
const float lscale = maxDist / float(numLightSamples);
const float densityFactor = 5;

void main()
{
	vec3 lightDir = normalize(LightPosition - pos.xyz) * lscale;
    float Tl = 1.0;
    vec3 lpos = pos.xyz + lightDir;

    for (int s = 0; s < numLightSamples; ++s) 
	{
        float ld = texture(Density, lpos / 5.0).r;
        Tl *= 1.0 - Absorption * stepSize * ld;
        if (Tl <= 0.01) 
			lpos += lightDir;
    }

	vec3 Li = LightIntensity * Tl;

	// (0, 0) < gl_TexCoord[0].xy < (1, 1)
	vec2 tpos = TexCoord0.xy - vec2(0.5, 0.5);

	// Discard fragments outside of the sphere
	//if(length(tpos) > 0.5)
	//	discard;

    FragColor = vec4(Li, texture(Density, pos.xyz / 5.0).r * (1.0 - (length(tpos) / 0.5)));

	//FragColor = vec4(vec3(1,1,1) * Li, texture(Density, pos.xyz / 5.0).r * 0.1);
	// FragColor = vec4(vec3(1,1,1) * texture(Density, pos.xyz / 5.0).r, 0.5);
}


