/*
 * File:       tetrahedron.c
 * Author:     vlad9486
 * Created on: 11.12.2014
 */

#include "tetrahedron.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float_t general_det(float_t* m, int n) {

    int i;
    int j;
    float_t* sub;
    float_t sign;
    float_t result;

    if (n == 1) {
        return *m;
    }
    else if (n > 1) {
        j = 0;
        result = 0.0f;
        sign = 1.0f;
        while (j < n) {
            sub = malloc((n - 1) * (n - 1) * sizeof(*sub));
            i = 1;
            while (i < n) {
                if (j > 0) {
                    memcpy(sub + (i - 1) * (n - 1), m + i * n, j * sizeof(*sub));
                }
                if (j < n - 1) {
                    memcpy(sub + (i - 1) * (n - 1) + j, m + i * n + j + 1, (n - 1 - j) * sizeof(*sub));
                }
                i++;
            }
            result += sign * m[j] * general_det(sub, n - 1);
            sign *= -1.0f;
            free(sub);
            j++;
        }
        return result;
    }
    else {
        return NAN;
    }
}

void general_submatrix(float_t* m, float_t* sub, int a, int b, int n) {

    int i;

    i = 0;
    while (i < b) {
        if (a > 0) {
            memcpy(sub + i * (n - 1), m + i * n, a * sizeof(*sub));
        }
        if (a < n - 1) {
            memcpy(sub + i * (n - 1) + a, m + i * n + a + 1, (n - 1 - a) * sizeof(*sub));
        }
        i++;
    }

    i = b + 1;
    while (i < n) {
        if (a > 0) {
            memcpy(sub + (i - 1) * (n - 1), m + i * n, a * sizeof(*sub));
        }
        if (a < n - 1) {
            memcpy(sub + (i - 1) * (n - 1) + a, m + i * n + a + 1, (n - 1 - a) * sizeof(*sub));
        }
        i++;
    }
}

void general_inverse(float_t* m, int n) {

    int i;
    int j;
    float_t* sub;
    float_t signx, signy;
    float_t* result;
    float_t d;

    d = general_det(m, n);
    result = malloc(n * n * sizeof(*result));
    sub = malloc((n - 1) * (n - 1) * sizeof(*sub));
    signy = 1.0f;
    i = 0;
    while (i < n) {
        signx = 1.0f;
        j = 0;
        while (j < n) {
            general_submatrix(m, sub, i, j, n);
            result[i * n + j] = signx * signy * general_det(sub, n - 1) / d;
            signx *= -1.0f;
            j++;
        }
        signy *= -1.0f;
        i++;
    }
    free(sub);
    memcpy(m, result, n * n * sizeof(*result));
    free(result);
}

void general_mul(float_t* a, float_t* b) {

    float_t c[16];
    int i;
    int j;
    int k;

    memset(&c, 0, sizeof(c));
    i = 0;
    while (i < 4) {
        j = 0;
        while (j < 4) {
            k = 0;
            while (k < 4) {
                c[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
                k++;
            }
            j++;
        }
        i++;
    }
    memcpy(a, &c, sizeof(c));
}

void general_rotate(float* matrix, int m, int n, float angle) {

    float c = cosf(angle);
    float s = sinf(angle);
    float temp[16];

    memset(&temp, 0, sizeof(temp));
    temp[0] = 1.0f;
    temp[5] = 1.0f;
    temp[10] = 1.0f;
    temp[15] = 1.0f;
    temp[m * 5] = c;
    temp[n * 5] = c;
    temp[m * 4 + n] = s;
    temp[n * 4 + m] = -s;

    general_mul(matrix, &temp);
}

GLuint vbo;
GLuint vao;

const int hypercube_size = 40;

void create_hypercube(tetrahedron4x4_t* hypercube) {

    point4_t vertices[16];
    int indices[4 * 5] = {
        7, 3, 5, 6,
        1, 0, 5, 3,
        2, 0, 3, 6,
        4, 0, 6, 5,
        0, 6, 5, 3
    };
    int indices_[8 * 8] = {
        0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe,
        0x1, 0x3, 0x5, 0x7, 0x9, 0xb, 0xd, 0xf,
        0x0, 0x1, 0x4, 0x5, 0x8, 0x9, 0xc, 0xd,
        0x2, 0x3, 0x6, 0x7, 0xa, 0xb, 0xe, 0xf,
        0x0, 0x1, 0x2, 0x3, 0x8, 0x9, 0xa, 0xb,
        0x4, 0x5, 0x6, 0x7, 0xc, 0xd, 0xe, 0xf,
        0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
        0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf,
    };
    int i;
    int j;
    int k;

    i = 0;
    while (i < 16) {
        vertices[i].c[0] = -0.5f + (float)((i >> 0) & 1);
        vertices[i].c[1] = -0.5f + (float)((i >> 1) & 1);
        vertices[i].c[2] = -0.5f + (float)((i >> 2) & 1);
        vertices[i].c[3] = -0.5f + (float)((i >> 3) & 1);
        i++;
    }

    i = 0;
    while (i < 8) {
        j = 0;
        while (j < 5) {
            k = 0;
            while (k < 4) {
                hypercube[i * 5 + j].v[k] = vertices[indices_[indices[j * 4 + k] + 8 * i]];
                k++;
            }
            j++;
        }
        i++;
    }
}

void prepare_tetrahedrons(shader_t* shader, tetrahedron4x4_t* tetrahedrons,
        int count) {

    GLint loc;

    exts.glGenVertexArrays(1, &vao);
    exts.glBindVertexArray(vao);
    exts.glGenBuffers(1, &vbo);
    exts.glBindBuffer(GL_ARRAY_BUFFER, vbo);
    exts.glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron4x4_t) * count,
            tetrahedrons, GL_STATIC_DRAW);

    getslot_shader(shader, &loc, "p0");
    exts.glEnableVertexAttribArray(loc);
    exts.glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE,
            sizeof(tetrahedron4x4_t), (const void*)(0 * sizeof(point4_t)));
    getslot_shader(shader, &loc, "p1");
    exts.glEnableVertexAttribArray(loc);
    exts.glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE,
            sizeof(tetrahedron4x4_t), (const void*)(1 * sizeof(point4_t)));
    getslot_shader(shader, &loc, "p2");
    exts.glEnableVertexAttribArray(loc);
    exts.glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE,
            sizeof(tetrahedron4x4_t), (const void*)(2 * sizeof(point4_t)));
    getslot_shader(shader, &loc, "p3");
    exts.glEnableVertexAttribArray(loc);
    exts.glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE,
            sizeof(tetrahedron4x4_t), (const void*)(3 * sizeof(point4_t)));
}

void cleanup_tetrahedrons() {

    exts.glDeleteBuffers(1, &vbo);
    exts.glDeleteVertexArrays(1, &vao);
}
