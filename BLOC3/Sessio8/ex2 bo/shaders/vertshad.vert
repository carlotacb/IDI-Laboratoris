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
// vec3 colFocus = vec3(0.8, 0.8, 0.8);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
// vec3 posFocus = vec3(1, 0, -1);  // en SCA

out vec3 fcolor;

out vec4 vertexSCOFS;
out vec3 NormSCOFS;
out vec3 matambFS;
out vec3 matdiffFs;
out vec3 matspecFS;
out float matshinFS;

void main()
{	
	gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    
    // fcolor = matdiff;
    
    // calcul del color LAMBERT:
    
    mat3 NormalMatrix = inverse(transpose(mat3(view*TG)));
    vec3 NormSCO = normalize(NormalMatrix*normal);
    vec4 vertexSCO = view * TG * vec4(vertex, 1.0);
    
    vertexSCOFS = vertexSCO;
    NormSCOFS = NormSCO;
    
    matambFS = matamb;
	matdiffFs = matdiff;
	matspecFS = matspec;
	matshinFS = matshin;   
    
}
