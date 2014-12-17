#version 330 core

in vec4 p0;
in vec4 p1;
in vec4 p2;
in vec4 p3;
out vec4 pp0;
out vec4 pp1;
out vec4 pp2;
out vec4 pp3;

void main(void) {

    pp0 = p0;
    pp1 = p1;
    pp2 = p2;
    pp3 = p3;
}
