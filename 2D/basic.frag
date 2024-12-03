#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;

uniform sampler2D uTex;
uniform vec3 tvCol;
uniform float brightness;
void main()
{
    outCol = vec4(tvCol*brightness, 1);
}