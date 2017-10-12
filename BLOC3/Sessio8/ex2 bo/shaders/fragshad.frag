#version 330 core

in vec4 vertexSCOFS;
in vec3 NormSCOFS;
in vec3 matambFS;
in vec3 matdiffFS;
in vec3 matspecFS;
in float matshinFS;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 colFocus;
uniform vec3 posFocus;
uniform vec3 llumAmbient;
uniform int tipusFocus;

out vec4 FragColor;

vec3 Lambert (vec3 NormSCOFS, vec3 L) 
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambFS;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCOFS) > 0)
      colRes = colRes + colFocus * matdiffFS * dot (L, NormSCOFS);
    return (colRes);
}

vec3 Phong (vec3 NormSCOFS, vec3 L, vec4 vertexSCOFS) 
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCOFS, L);

    // Calculem R i V
    if (dot(NormSCOFS,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCOFS); // equival a: normaliz (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertexSCOFS.xyz);

    if ((dot(R, V) < 0) || (matshinFS == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinFS);
    return (colRes + matspecFS * colFocus * shine); 
}

void main()
{	
	vec4 focusSCO;
	    
    /*if (tipusFocus == 1) {
		focusSCO = view * TG * vec4(posFocus, 1.0);
	}
    if (tipusFocus == 2) {
		focusSCO = view * vec4(posFocus, 1.0);
	}
	if (tipusFocus == 3) {*/
		focusSCO = vec4(posFocus, 1.0);
	//}
    
    vec4 L = focusSCO - vertexSCOFS;
    vec3 Lxyz = normalize(L.xyz); 
    // Es calcula en vec4 com tot, pero lambert demana un vertex de 3 per tant es normalitza en 3 per que sigui coherent.
    
    // fcolor = Lambert(NormSCO, Lxyz);  
    
    // calcul de color PHONG:
    
    //vec3 fcolor = Phong(NormSCOFS, Lxyz, vertexSCOFS);
	
	FragColor = vec4(Phong(NormSCOFS, Lxyz, vertexSCOFS),1);
}
