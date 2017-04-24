-- Vertex.GL3

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 DiffuseMaterial;

in vec4 Position;
in vec4 Color;

out vec4 color;

void main()
{
    gl_Position = Modelview * Position;
	color = Color;
}

-- Geometry.GL3

#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable 

uniform mat4 ProjectionGeom;

in vec4 color[];

out vec4 eye_position;
out vec2 TexCoord0;
out vec4 pos;
out vec4 vFragColor;

void main()
{
    float halfsize = 0.1;

	eye_position = gl_PositionIn[0];
	pos = gl_PositionIn[0];
	vFragColor = color[0];
	
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

in vec4 pos;
in vec2 TexCoord0;
in vec4 eye_position;
in vec4 vFragColor;

out vec4 FragColor;

void main()
{
	vec2 tpos = TexCoord0.xy - vec2(0.5, 0.5);

    //FragColor = vec4(vFragColor.rgb, 0.1 * (1.0 - (length(tpos) / 0.5)));
	float x = length(tpos) / 0.5;
	FragColor = vec4(vFragColor.rgb, 0.1 * pow(2.718, -6.14*x*x));
}


