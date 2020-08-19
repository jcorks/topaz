#include <topaz/compat.h>
#include <topaz/entity.h>
#include <topaz/topaz.h>
#include <topaz/component.h>
#include <topaz/system.h>
#include <string.h>

typedef struct {
    int attach;
    int detach;
    int step;
    int prestep;
    int draw;
    int predraw;
    int remove;
} TestEntity;



static void on_attach(topazEntity_t * src, TestEntity * e) {
    e->attach++;
}

static void on_detach(topazEntity_t * src, TestEntity * e) {
    e->detach++;
}


static void on_remove(topazEntity_t * src, TestEntity * e) {
    e->remove++;
}

static void on_pre_step(topazEntity_t * src, TestEntity * e) {
    e->prestep++;
}

static void on_step(topazEntity_t * src, TestEntity * e) {
    e->step++;
}

static void on_pre_draw(topazEntity_t * src, TestEntity * e) {
    e->predraw++;
}

static void on_draw(topazEntity_t * src, TestEntity * e) {
    e->draw++;
}


int test__entity_simple() {   

    topaz_t * ctx = topaz_context_create_empty();
    TestEntity data;
    memset(&data, 0, sizeof(TestEntity));
    topazEntity_Attributes_t attr;

    attr.on_attach = (topaz_entity_attribute_callback)on_attach;    
    attr.on_detach = (topaz_entity_attribute_callback)on_detach;
    attr.on_remove = (topaz_entity_attribute_callback)on_remove;    
    attr.on_pre_step = (topaz_entity_attribute_callback)on_pre_step;
    attr.on_step = (topaz_entity_attribute_callback)on_step;
    attr.on_pre_draw = (topaz_entity_attribute_callback)on_pre_draw;
    attr.on_draw = (topaz_entity_attribute_callback)on_draw;
    attr.userData = &data;

    topazEntity_t * e = topaz_entity_create_with_attributes(ctx, &attr);    
    if (!topaz_entity_is_valid(e)) return 1;
    if (topaz_array_get_size(topaz_entity_get_children(e)) != 0) return 2;
    if (topaz_entity_get_parent(e) != TOPAZ_ENULL) return 3;


    topaz_entity_step(e);
    if (data.prestep != 1) return 4;
    if (data.step != 1)    return 5;

    topaz_entity_draw(e);
    if (data.predraw != 1) return 6;
    if (data.draw != 1)    return 7;

    topaz_entity_set_stepping(e, FALSE);
    topaz_entity_set_drawing(e, FALSE);



    topaz_entity_step(e);
    if (data.prestep != 1) return 4;
    if (data.step != 1)    return 5;

    topaz_entity_draw(e);
    if (data.predraw != 1) return 6;
    if (data.draw != 1)    return 7;




    topazEntity_t * child = topaz_entity_create_with_attributes(ctx, &attr);
    topaz_entity_attach(e, child);

    if (topaz_entity_get_parent(child) != e) return 8;
    if (topaz_array_get_size(topaz_entity_get_children(e)) != 1) return 9;

    topaz_entity_step(e);
    if (data.prestep != 1) return 8;
    if (data.step != 1)    return 9;

    topaz_entity_draw(e);
    if (data.predraw != 1) return 10;
    if (data.draw != 1)    return 11;


    topaz_entity_set_stepping(e, TRUE);
    topaz_entity_set_drawing(e, TRUE);


    topaz_entity_step(e);
    if (data.prestep != 3) return 12;
    if (data.step != 3)    return 13;

    topaz_entity_draw(e);
    if (data.predraw != 3) return 14;
    if (data.draw != 3)    return 15;


    topaz_entity_position(e)->x = 1;

    topaz_entity_step(e);
    if (data.prestep != 5) return 16;
    if (data.step != 5)    return 17;

    topaz_entity_draw(e);
    if (data.predraw != 5) return 18;
    if (data.draw != 5)    return 19;

    if (topaz_entity_get_global_position(child).x < .9) {
        return 20;
    }



    topaz_entity_remove(e);
    if (topaz_entity_get_parent(child) != TOPAZ_ENULL) return 21;


    if (topaz_entity_is_valid(e)) return 99;    
    topaz_context_destroy(ctx);
    

    return 0;
}





typedef struct {
    int handler1;
    int handler2;
    int step;
    int draw;
    int detach;
    int attach;
    int remove;
} ComponentType;

static int event_handler1(topazComponent_t * c, void * dataEvent, topazEntity_t * source, void * sourceEvent) {
    ComponentType *cData = topaz_component_get_attributes(c)->userData;
    cData->handler1 = (int)(intptr_t)dataEvent;
    return TRUE;
}

static int event_handler2(topazComponent_t * c, void * dataEvent, topazEntity_t * source, void * sourceEvent) {
    ComponentType *cData = topaz_component_get_attributes(c)->userData;
    cData->handler2 = (int)(intptr_t)sourceEvent;
    return TRUE;
}




static void component_step(topazComponent_t * c, ComponentType * data) {
    topaz_component_emit_event(c, TOPAZ_STR_CAST("event2"), TOPAZ_ENULL, (void*)(intptr_t)8);
    data->step++;
}

static void component_draw(topazComponent_t * c, ComponentType * data) {
    data->draw++;
}

static void component_attach(topazComponent_t * c, ComponentType * data) {
    data->attach++;
}

static void component_detach(topazComponent_t * c, ComponentType * data) {
    data->detach++;
}

static void component_destroy(topazComponent_t * c, ComponentType * data) {
    data->remove++;
}





int test__entity_advanced() {
    topaz_t * ctx = topaz_context_create_empty();
    topazComponent_t * c = topaz_component_create(TOPAZ_STR_CAST("COMPONENT"), ctx);
    topazComponent_Attributes_t attrib;
    memset(&attrib, 0, sizeof(topazComponent_Attributes_t));
    
    attrib.on_step = (topaz_component_attribute_callback)component_step;
    attrib.on_draw = (topaz_component_attribute_callback)component_draw;
    attrib.on_detach = (topaz_component_attribute_callback)component_detach;
    attrib.on_attach = (topaz_component_attribute_callback)component_attach;
    attrib.on_destroy = (topaz_component_attribute_callback)component_destroy;
    ComponentType cdata;
    memset(&cdata, 0, sizeof(ComponentType));
    attrib.userData = &cdata;    

    topaz_component_set_attributes(c, &attrib);


    topaz_component_install_event(c, TOPAZ_STR_CAST("event1"), event_handler1, (void*)(intptr_t)1);
    topaz_component_install_event(c, TOPAZ_STR_CAST("event2"), event_handler2, (void*)(intptr_t)2);


    topaz_component_emit_event_anonymous(c, TOPAZ_STR_CAST("event1"));
    topaz_component_emit_event_anonymous(c, TOPAZ_STR_CAST("event2"));

    if (cdata.handler1 != 1 || cdata.handler2 != 0) {
        return 1;
    }

    topaz_component_emit_event(c, TOPAZ_STR_CAST("event1"), TOPAZ_ENULL, (void*)(intptr_t)3);
    topaz_component_emit_event(c, TOPAZ_STR_CAST("event2"), TOPAZ_ENULL, (void*)(intptr_t)4);

    if (cdata.handler1 != 1 || cdata.handler2 != 4) {
        return 2;
    }


    topazEntity_t * e = topaz_entity_create(ctx);
    topaz_entity_add_component(e, c);

    if (cdata.attach != 1) {
        return 3;
    }


    topaz_entity_step(e);
    topaz_entity_draw(e);

    if (cdata.step != 1 ||
        cdata.draw != 1)
        return 4;

    if (cdata.handler2 != 8) {
        return 5;
    }





    topaz_entity_remove(e);

    if (cdata.remove != 1 ||
        cdata.detach != 1) 
        return 6;

    return 0;
    topaz_context_destroy(ctx);
}
