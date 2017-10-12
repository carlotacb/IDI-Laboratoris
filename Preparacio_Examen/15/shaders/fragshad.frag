#version 330 core

in vec3  vertexFS;
in vec3  normalFS;
in vec3  matambFS;
in vec3  matdiffFS;
in vec3  matspecFS;
in float matshinFS;
in vec3  llumAmbientFS;

uniform vec3 colFocus;
uniform vec3 posFocus;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

out vec4 FragColor;

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbientFS * matambFS;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiffFS * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshinFS == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shineFS = pow(max(0.0, dot(R, V)), matshinFS);
    return (colRes + matspecFS * colFocus * shineFS);
}

void main()
{	
        vec4 focusSCO = view * vec4(posFocus, 1.0);
        mat3 NormalMatrix = inverse(transpose(mat3(view * TG)));
        vec3 normalSCO    = normalize(NormalMatrix * normalFS);
        vec4 vertexSCO    = view * TG * vec4(vertexFS, 1.0);
        vec4 L            = focusSCO - vertexSCO;
        vec3 Lxyz         = normalize(L.xyz);
        // Es calcula en vec4 com tot, pero lambert  demana un vertex
        // de 3 per tant es normalitza en 3 per que sigui coherent.

        // fcolor = Lambert(NormSCO, Lxyz);
        FragColor = vec4 (Phong(normalSCO, Lxyz, vertexSCO), 1.0);
}
