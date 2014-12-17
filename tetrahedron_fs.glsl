#version 330 core

/*uniform sampler2D texture;
in vec2 texcoord;*/
out vec4 color;
in vec3 normal;
in vec3 light;
in vec4 pos;

void main(void) {

    float l;
    vec3 color0 = vec3(0.8, 0.6, 0.1);
    vec3 color1 = vec3(0.0, 0.4, 0.5);
    float f = cos(pos[0] * 40.0) * cos(pos[1] * 40.0) * cos(pos[2] * 40.0);
    vec3 color01 = color0 * f + color1 * sqrt(1 - f * f * f);

    l = abs(dot(light, normal));
    color = vec4(color01 * l, 1.0);/*texture2D(texture, texcoord);*/
}
