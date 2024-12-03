#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; //teksturna jedinica
uniform float y;
uniform int activeRect;
uniform float brightness;
void main()
{
	vec4 texColor;
	if(activeRect == 1 && y < 0.7 && y >= 0.45)  
		texColor = vec4(1,1,1,1) - texture(uTex, chTex);
	else if(activeRect == 2 && y < 0.45 && y > 0.2) 
		texColor = vec4(1,1,1,1) - texture(uTex, chTex);	
	else if(activeRect == 3 && y <= 0.2 && y > -0.15)  
		texColor = vec4(1,1,1,1) - texture(uTex, chTex);
	else if(activeRect == 4 && y < -0.15 && y > -0.4) 
		texColor = vec4(1,1,1,1) - texture(uTex, chTex);	
	else if(activeRect == 5 && y <= -0.4 && y > -0.7)  
		texColor = vec4(1,1,1,1) - texture(uTex, chTex);
	else
		texColor = texture(uTex, chTex);
	outCol = texColor * brightness;
}