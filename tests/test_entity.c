#include <topaz/compat.h>
#include <topaz/entity.h>
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

    topazEntity_t * e = topaz_entity_create_with_attributes(&attr);    
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




    topazEntity_t * child = topaz_entity_create_with_attributes(&attr);
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

    

    return 0;
}


int test__entity_advanced() {

    return 0;
}
