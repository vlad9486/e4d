/*
 * File:       tetrahedron.c
 * Author:     vlad9486
 * Created on: 11.12.2014
 */

#include "tetrahedron.h"

void draw_tetrahedrons(shader_t* shader, float* matrix, float* point,
        tetrahedron4x4_t* tetrahedrons, int count) {

    GLuint loc;
    GLuint vao, vbo;

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

    getuniformslot_shader(shader, &loc, "matrix4");
    exts.glUniformMatrix4fv(loc, 16, GL_FALSE, matrix);
    getuniformslot_shader(shader, &loc, "invmatrix4");
    exts.glUniformMatrix4fv(loc, 16, GL_FALSE, matrix);/*FIXME*/
    getuniformslot_shader(shader, &loc, "point4");
    exts.glUniform4fv(loc, 4, point);

    glDrawArrays(GL_POINTS, 0, count);

    exts.glDeleteBuffers(1, &vbo);
    exts.glDeleteVertexArrays(1, &vao);
}
