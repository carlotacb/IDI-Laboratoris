#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

// Valors per als components que necessitem dels focus de llum
// vec3 colFocus = vec3(0.8, 0.8, 0.8);
// vec3 posFocus = vec3(1, 0, -1);  // en SCA

out vec3 normalFS;
out vec3 vertexFS;
out vec3 matdiffFS;
out vec3 matshinFS;
out vec3 matspecFS;


void main()
{	
	gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    
    matambFS = matamb;
    matdiffFS = matdiff;
    matspecFS = matspec;
    matshinFS = matshin;
    
    normalFS = normal;
    vertexFS = vertex;
    
    
    
    
}
