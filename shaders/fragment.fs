#version 330 core
out vec4 FragColor;

in vec3 VertColor;
in vec2 TexCoord;

uniform sampler2D texContainer;

void main() {
    FragColor = texture(texContainer, TexCoord);
}
