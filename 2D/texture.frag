#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; //teksturna jedinica
uniform float brightness;
void main()
{
	outCol = texture(uTex, chTex) * brightness;
}