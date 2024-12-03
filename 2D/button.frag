#version 330 core

out vec4 outCol;

uniform vec3 buttonCol;
void main()
{
	outCol = vec4(buttonCol, 1.0);
}