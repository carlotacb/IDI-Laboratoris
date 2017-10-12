#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

out vec3 fcolor;
out vec3  vertexFS;
out vec3  normalFS;
out vec3  matambFS;
out vec3  matdiffFS;   // RJA out vec3 matdiffFs;
out vec3  matspecFS;
out float matshinFS;
out vec3  llumAmbientFS;


void main()
{	
    vertexFS  = vertex;
    normalFS  = normal;
    matambFS  = matamb;
    matdiffFS = matdiff;
    matspecFS = matspec;
    matshinFS = matshin;
    llumAmbientFS = llumAmbient;

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
