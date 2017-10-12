#version 330 core

in vec3  vertex;
in vec3  normal;
in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

// RJA CODI QUE NO HI ERA
uniform vec3 posFocus;     
uniform vec3 colFocus;     
uniform int  tipusFocus;   // 1: SCA, 2: SCO, 3: SCM

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

// Valors per als components que necessitem dels focus de llum
// vec3 colFocus = vec3(0.8, 0.8, 0.8);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
// vec3 posFocus = vec3(1, 0, -1);  // en SCA


// RJA
// out vec3 fcolor;
out vec3  vertexFS;
out vec3  normalFS;
out vec3  matambFS;
out vec3  matdiffFS;   // RJA out vec3 matdiffFs;
out vec3  matspecFS;
out float matshinFS;
out vec3  llumAmbientFS;

void main()
{	
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);

    // RJA CAL PASAR_HO TOT AL fragment shader llevat de
    // les assignacions input/output dels atributs
    
    vertexFS  = vertex;
    normalFS  = normal;
    matambFS  = matamb;
    matdiffFS = matdiff;
    matspecFS = matspec;
    matshinFS = matshin;   

    // RJA CODI AFEGIT
    llumAmbientFS = llumAmbient;
}
