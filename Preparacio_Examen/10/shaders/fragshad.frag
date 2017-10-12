#version 330 core

in vec3 fcolor;
out vec4 FragColor;

uniform int pintaralles;

void main()
{

    FragColor = vec4(fcolor,1);

    if (pintaralles == 1) {

        int num = int(gl_FragCoord.y)/10;

        if(num%2==0){

            FragColor = vec4 (0.0 ,0.0 ,0.0 ,1.0);
        }
    }
}
