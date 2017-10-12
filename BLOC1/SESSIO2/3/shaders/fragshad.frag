#version 330 core

out vec4 FragColor;

void main() {

	//FragColor = vec4(f_color,1);  
	
	if(gl_FragCoord.x < 358.) {
      
	if(gl_FragCoord.y > 310.) FragColor = vec4(1, 0, 0, 1);
	else FragColor = vec4(1, 1, 0, 1);
	}
      else {
	
	if(gl_FragCoord.y > 310.) FragColor = vec4(0, 0, 1, 1);
	else FragColor = vec4(0, 1, 0, 1);
	
	}
      
      for(float i = 0; i < 730; i=i+10) {
	
	if((gl_FragCoord.y > i) && (gl_FragCoord.y < i+5)) FragColor = vec4(0.5, 0.7, 1.0, 1.0);
	
	}
    
}
