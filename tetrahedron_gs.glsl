#version 420 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 world;
uniform mat4 invworld;
uniform vec4 world_shift;

uniform mat4 view;
uniform mat4 invview;
uniform vec4 view_shift;

in vec4 pp0[];
in vec4 pp1[];
in vec4 pp2[];
in vec4 pp3[];
out vec3 normal;
out vec3 light;
out vec4 pos;

struct cut_result_t {
    vec4 p[4];
};

cut_result_t cut_(vec4 n, vec4 p, vec4 m[4]) {

    cut_result_t res;
    const float eps = 0.00001;
    int i;
    int j;
    float t;
    float s;
    int indices_0[6] = {1, 2, 0, 0, 1, 2};
    int indices_1[6] = {2, 0, 1, 3, 3, 3};
    int fst;
    int scd;
    int flag;

    i = 0;
    j = 0;
    flag = 0;
    while (j < 6) {
        fst = indices_0[j];
        scd = indices_1[j];
        s = dot(m[scd] - m[fst], n);
        if ((s < -eps) || (s > eps)) {
            t = dot(p - m[fst], n) / s;
            if ((t >= 0.0) && (t <= 1.0)) {
                res.p[i] = m[fst] + t * (m[scd] - m[fst]);
                i++;
            }
        }
        if (i == 4) {
            flag = 1;
            break;
        }
        j++;
    }
    if (flag == 0) {
        res.p[3] = res.p[2];
    }

    return res;
}

void main(void) {

    mat4 scale = mat4(
            vec4(0.09, 0.0, 0.0, 0.0),
            vec4(0.0, 0.16, 0.0, 0.0),
            vec4(0.0, 0.0, 1.0, 0.0),
            vec4(0.0, 0.0, 0.0, 1.0));
    cut_result_t cutted;
    int indices[6] = {0, 1, 2, 0, 2, 3};
    int i;
    vec4 temp;
    vec4 a;
    vec4 b;
    vec4 l;
    vec4 tetrahedron[4];

    tetrahedron[0] = world * pp0[0] + world_shift;
    tetrahedron[1] = world * pp1[0] + world_shift;
    tetrahedron[2] = world * pp2[0] + world_shift;
    tetrahedron[3] = world * pp3[0] + world_shift;

    cutted = cut_(invview[3], view_shift, tetrahedron);
    tetrahedron[0] = view * (cutted.p[0] - view_shift);
    tetrahedron[1] = view * (cutted.p[1] - view_shift);
    tetrahedron[2] = view * (cutted.p[2] - view_shift);
    tetrahedron[3] = view * (cutted.p[3] - view_shift);

    a = tetrahedron[2] - tetrahedron[0];
    b = tetrahedron[2] - tetrahedron[1];
    normal = normalize(cross(vec3(a[0], a[1], a[2]), vec3(b[0], b[1], b[2])));
    l = view * vec4(1.0, 0.0, -2.0, 0.0);
    light = normalize(vec3(l[0], l[1], l[2]));

    i = 0;
    while (i < 6) {
        temp = scale * tetrahedron[indices[i]];
        pos = invworld * (cutted.p[indices[i]] - world_shift);
        gl_Position = vec4(temp[0], temp[1], temp[2], 1.0);
        EmitVertex();
        if ((i % 3) == 2) {
            EndPrimitive();
        }
        i++;
    }
}
