#version 420 core

in vec4 p0;
in vec4 p1;
in vec4 p2;
in vec4 p3;
out mat4 pp;

void main(void) {

    pp = mat4(p0, p1, p2, p3);
}
