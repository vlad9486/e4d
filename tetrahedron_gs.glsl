#version 420 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 matrix4;
uniform mat4 invmatrix4;
uniform vec4 point4;

in mat4 pp[];

void main(void) {

    const float eps = 1e-5;
    int i;
    int j;
    vec4 point;
    float t;
    float s;
    mat4 m;
    int indices_0[6] = {0, 1, 2, 0, 1, 2};
    int indices_1[6] = {3, 3, 3, 1, 2, 0};
    int fst;
    int scd;

    m = pp[0];
    i = 0;
    j = 0;

    while (j < 6) {
        fst = indices_0[j];
        scd = indices_1[j];
        s = dot(m[scd] - m[fst], matrix4[3]);
        if ((s < -eps) || (s > eps)) {
            t = dot(point4 - m[fst], matrix4[3]) / s;
            if ((t > 0.0f) && (t < 1.0f)) {
                point = invmatrix4 * (m[fst] + t * (m[scd] - m[fst]));
                gl_Position = vec4(point[0], point[1], 0.0, 1.0);
                EmitVertex();
                i++;
                if (i == 3) {
                    EndPrimitive();
                    return;
                }
            }
        }
        j++;
    }
}
