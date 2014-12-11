#version 420 core

/*uniform mat4 w;
uniform mat4 v;
uniform mat4 p;*/
in vec2 position;
out vec2 texcoord;

void main() {

    texcoord = position * vec2(4.0, 1.0);
    gl_Position = vec4(position, 0.0, 1.0);
}
