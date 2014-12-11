/*
 * File:       tetrahedron.h
 * Author:     vlad9486
 * Created on: 11.12.2014
 */

#ifndef TETRAHEDRON_H_
#define TETRAHEDRON_H_

#include <togl.h>

struct point4_t {
    float x, y, z, w;
};
typedef struct point4_t point4_t;

struct tetrahedron4x4_t {
    point4_t p0, p1, p2, p3;
};
typedef struct tetrahedron4x4_t tetrahedron4x4_t;

void draw_tetrahedrons(shader_t* shader, float* matrix, float* point,
        tetrahedron4x4_t* tetrahedrons, int count);

#endif /* TETRAHEDRON_H_ */
