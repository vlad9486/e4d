/*
 * main.c
 *
 *  Created on: 19 oct 2014
 *      Author: vlad
 */

#include <stdlib.h>
#include <togl.h>

#include "tetrahedron.h"

struct tuio_client_t* client;
struct tg_display_t* display;
struct tg_mode_t* modes;
int modes_count;
int running = 0;

shader_t* shader;
/*texture_t* texture;
imagebuffer_t* image;
sampler_t* sampler;
GLuint loc;*/

float world[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

float invworld[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

float view[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

void redraw();

int initialize() {

    GLint loc;
    float point[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    tetrahedron4x4_t hypercube[hypercube_size * 2];

    glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, modes->hdisplay, modes->vdisplay);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    running = 1;

    create_shader(&shader);
    load_shader(shader, "tetrahedron_vs.glsl", shader_type_vertex);
    load_shader(shader, "tetrahedron_gs.glsl", shader_type_geometry);
    load_shader(shader, "tetrahedron_fs.glsl", shader_type_fragment);
    shaders_loaded_to_shader(shader);

    bind_shader(shader);

    create_hypercube(hypercube);
    create_hypercube(&hypercube[hypercube_size]);
    prepare_tetrahedrons(shader, (tetrahedron4x4_t *)&hypercube,
            hypercube_size * 2);

    getuniformslot_shader(shader, &loc, "view_shift");
    exts.glUniform4fv(loc, 1, point);
    getuniformslot_shader(shader, &loc, "view");
    general_rotate(view, 1, 2, 3.14f / 6.0f);
    general_rotate(view, 1, 3, 3.14f / 6.0f);
    general_rotate(view, 0, 1, 3.14f / 3.0f);
    general_rotate(view, 2, 3, 3.14f / 5.0f);
    exts.glUniformMatrix4fv(loc, 1, GL_FALSE, view);
    getuniformslot_shader(shader, &loc, "invview");
    general_inverse(view, 4);
    exts.glUniformMatrix4fv(loc, 1, GL_FALSE, view);

    getuniformslot_shader(shader, &loc, "world");
    exts.glUniformMatrix4fv(loc, 1, GL_FALSE, world);
    getuniformslot_shader(shader, &loc, "invworld");
    exts.glUniformMatrix4fv(loc, 1, GL_FALSE, invworld);

    redraw();

    /*create_imagebuffer(&image);
    load_imagebuffer(image, "texture.tga", imagefile_type_tga);
    create_texture(&texture);
    load_texture(texture, image);
    create_sampler(&sampler);

    getuniformslot_shader(shader, &loc, "texture");
    bind_texture(texture, loc);
    bind_sampler(sampler, texture);*/

    return 0;
}

int finalize() {

    /*bind_texture(texture, -1);
    destroy_imagebuffer(image);
    destroy_texture(texture);*/

    cleanup_tetrahedrons();
    destroy_shader(shader);

    return 0;
}

void redraw() {

    GLint loc;
    float pos0[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float pos1[4] = {1.5f, 0.0f, 0.0f, 0.0f};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getuniformslot_shader(shader, &loc, "world_shift");
    exts.glUniform4fv(loc, 1, pos0);
    glDrawArrays(GL_POINTS, 0, hypercube_size);

    getuniformslot_shader(shader, &loc, "world_shift");
    exts.glUniform4fv(loc, 1, pos1);
    glDrawArrays(GL_POINTS, hypercube_size, hypercube_size);

    tg_swap_display(display, 1);
}

int keyboard(unsigned long event) {

    if (event != 0) {
        running = 0;
    }

    return 0;
}

int mouse(unsigned long event) {

    if (event != 0) {
        running = 0;
    }

    return 0;
}

int idle(unsigned long event) {

    return 0;
}

int main(int argc, char* argv[]) {

    init_log("log");
    tuio_initialize(&client, write_log);
    modes_count = 8;
    tuio_acquire_display(client, &display, 0, 0, &modes, &modes_count);
    tg_change_mode(display, modes, 1);
    tuio_setcallback(client, ti_keyboard, 0, keyboard);
    tuio_setcallback(client, ti_mouse, 0, mouse);
    tuio_setcallback(client, ti_idle, 0, idle);
    tg_bind_opengl(display, 1);
    set_current_display(display);

    initialize();
    while (running) {
        tuio_loop(client, display);
    }
    finalize();

    tg_bind_opengl(display, 0);
    tuio_release_display(client, 0, 0);

    free(modes);
    tuio_finalize(client);
    final_log();

    return EXIT_SUCCESS;
}
