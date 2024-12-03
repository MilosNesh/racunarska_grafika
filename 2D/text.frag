#version 330 core

in vec2 chTex; // Koordinate teksture
out vec4 outCol;

uniform sampler2D uTextures[16]; // Niz tekstura
uniform int selectedTexture;    // Indeks trenutno odabrane teksture

void main() {
    outCol = texture(uTextures[selectedTexture], chTex);
}
