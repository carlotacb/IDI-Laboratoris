#version 330 core

in vec3 vertex;
// in vec3 color;

// out vec3 f_color;

uniform mat4 TG;

void main()  {
//     f_color = color;
    
    gl_Position = TG * vec4(0.5*vertex, 1.0);
}
