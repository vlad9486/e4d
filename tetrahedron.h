/*
 * File:       tetrahedron.h
 * Author:     vlad9486
 * Created on: 11.12.2014
 */

#ifndef TETRAHEDRON_H_
#define TETRAHEDRON_H_

#include <togl.h>

union point4_t {
    struct {
        float x, y, z, w;
    };
    float c[4];
};
typedef union point4_t point4_t;

union tetrahedron4x4_t {
    struct {
        point4_t p0, p1, p2, p3;
    };
    point4_t v[4];
};
typedef union tetrahedron4x4_t tetrahedron4x4_t;

const int hypercube_size;

void general_rotate(float* matrix, int m, int n, float angle);
void general_inverse(float* m, int n);

void create_hypercube(tetrahedron4x4_t* hypercube);
void prepare_tetrahedrons(shader_t* shader, tetrahedron4x4_t* tetrahedrons,
        int count);
void cleanup_tetrahedrons();

#endif /* TETRAHEDRON_H_ */
