#include "rectangle.h"
#include <topaz/all.h>
#include <stdio.h>
#include <string.h>
static void on_active(topazInput_t * input, int key, float v, void * e) {
    if (key == topazKey_up && v > 0)
        topaz_entity_position(e)->y -= 10;
    if (key == topazKey_down && v > 0)
        topaz_entity_position(e)->y += 10;
    if (key == topazKey_left && v > 0)
        topaz_entity_position(e)->x -= 10;
    if (key == topazKey_right && v > 0)
        topaz_entity_position(e)->x += 10;

    printf("%d %f\n", key, v);


}

void rectangle__on_step(topazEntity_t * e, void * ctx) {
    //topaz_entity_position(e)->x = topaz_input_mouse_x(input);
    //topaz_entity_position(e)->y = topaz_input_mouse_y(input);


        

    topaz_entity_rotation(e)->x += .0004;
    topaz_entity_rotation(e)->y += .0005;
    topaz_entity_rotation(e)->z += .0003;
}


topazEntity_t * rectangle_create(topaz_t * ctx) {
    topazEntity_Attributes_t attr = {0};

    attr.on_step = rectangle__on_step;
    attr.userData = ctx;
    topazEntity_t * e = topaz_entity_create_with_attributes(ctx, &attr);        
    topazComponent_t * shape = topaz_shape2d_create(ctx);
    topaz_shape2d_set_color(shape, topaz_color_from_string(TOPAZ_STR_CAST("cyan")));


    topazViewManager_t * vm = topaz_context_get_view_manager(ctx);
    topazDisplay_t * d = topaz_view_manager_get_default(vm);
    topazRenderer_Framebuffer_t * fb = topaz_display_get_framebuffer(
        d, 
        topazDisplay_Framebuffer_A
    );
    topaz_shape2d_form_rectangle(
        shape,
        topaz_renderer_framebuffer_get_width(fb) / 2,
        topaz_renderer_framebuffer_get_height(fb) / 2
    );
    topazVector_t center = topaz_vector_from_xyz(
        topaz_renderer_framebuffer_get_width(fb) / 4,
        topaz_renderer_framebuffer_get_height(fb) / 4,
        0
    );
    topaz_shape2d_set_center(
        shape,
        &center
    );

    topazInput_Listener_t l = {};
    l.userData = e;
    l.on_active = on_active;
    topaz_input_add_keyboard_listener(topaz_context_get_input(ctx), &l);    


    topaz_entity_position(e)->x += topaz_renderer_framebuffer_get_width(fb) / 2;
    topaz_entity_position(e)->y += topaz_renderer_framebuffer_get_height(fb) / 2;


    topaz_entity_add_component(e, shape);


    return e;
}

