#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 u_Color; uniform sampler2D u_Texture; uniform int u_HasTexture;

void main() {
    if(u_HasTexture == 1) {
        FragColor = texture(u_Texture, TexCoord) * u_Color;
    } else {
        FragColor = u_Color;
    }
}