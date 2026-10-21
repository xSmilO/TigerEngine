#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform; 

void main() {
    // Apply the camera's view projection matrix AFTER the object's world transform
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}