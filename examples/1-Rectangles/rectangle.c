#include "rectangle.h"
#include <topaz/components/shape2d.h>

void rectangle__on_step(topazEntity_t * e, void * data) {
    topaz_entity_rotation(e)->x += .4;
    topaz_entity_rotation(e)->y += .5;
    topaz_entity_rotation(e)->z += .3;
}


topazEntity_t * rectangle_create(topaz_t * ctx) {
    topazEntity_Attributes_t attr = {0};

    attr.on_step = rectangle__on_step;
    
    topazEntity_t * e = topaz_entity_create_with_attributes(ctx, &attr);    
    
    topazComponent_t * shape = topaz_shape2d_create(ctx);
    topaz_shape2d_set_color(shape, topaz_color_from_string(TOPAZ_STR_CAST("cyan")));
    topaz_shape2d_form_rectangle(shape, 64, 64);


    topaz_entity_add_component(e, shape);
    return e;
}

