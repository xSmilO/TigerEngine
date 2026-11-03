#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_UVRect;

void main() {

    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 0.0, 1.0);
    TexCoord = mix(u_UVRect.xy, u_UVRect.zw, aTexCoord);
}
