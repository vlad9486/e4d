#version 420 core

uniform sampler2D texture;
in vec2 texcoord;
out vec4 color;

void main() {

    color = texture2D(texture, texcoord);
}
