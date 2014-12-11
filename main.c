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

int initialize() {

    shader_t* shader;
    /*texture_t* texture;
    imagebuffer_t* image;
    sampler_t* sampler;
    GLuint loc;*/
    float matrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
    };
    float point[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    tetrahedron4x4_t tetrahedrons[2] = {
            {
                    {0.8f, 0.0f, 0.0f, -0.5f},
                    {0.0f, 0.8f, 0.0f, -0.5f},
                    {0.4f, 0.4f, 0.0f, -0.5f},
                    {0.8f, 0.8f, 0.0f, 0.5f}
            },
            {
                    {0.5f, 0.0f, 0.0f, -0.5f},
                    {0.0f, 0.5f, 0.0f, -0.5f},
                    {0.0f, 0.0f, 0.0f, -0.5f},
                    {0.0f, 0.0f, 0.0f, 0.5f}
            },
    };

    glClearColor(0.25f, 0.35f, 0.75f, 1.0f);
    glClearDepth(1.0f);
    glViewport(0, 0, modes->hdisplay, modes->vdisplay);
    running = 1;

    create_shader(&shader);
    load_shader(shader, "tetrahedron_vs.glsl", shader_type_vertex);
    load_shader(shader, "tetrahedron_gs.glsl", shader_type_geometry);
    load_shader(shader, "tetrahedron_fs.glsl", shader_type_fragment);
    shaders_loaded_to_shader(shader);

    bind_shader(shader);

    /*create_imagebuffer(&image);
    load_imagebuffer(image, "texture.tga", imagefile_type_tga);
    create_texture(&texture);
    load_texture(texture, image);
    create_sampler(&sampler);

    getuniformslot_shader(shader, &loc, "texture");
    bind_texture(texture, loc);
    bind_sampler(sampler, texture);*/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_tetrahedrons(shader, (float *)&matrix, (float *)&point,
            (tetrahedron4x4_t *)&tetrahedrons, 1);
    tg_swap_display(display, 1);

    /*bind_texture(texture, -1);
    destroy_imagebuffer(image);
    destroy_texture(texture);*/

    destroy_shader(shader);

    return 0;
}

int finalize() {

    return 0;
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
