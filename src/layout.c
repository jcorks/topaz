#include <topaz/compat.h>
#include <topaz/component.h>
#include <topaz/components/text2d.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/layout.h>
#include <topaz/topaz.h>
#include <topaz/entity.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif
typedef struct {
    topazLayout_Rectangle_t bounds;
    topazLayout_Rectangle_t innerSpace;
    topazLayout_Rectangle_t outerSpace;
    
    
    topazLayout_Rectangle_t marginOffset;
    topazLayout_Rectangle_t paddingOffset;
    
    topazLayout_Attributes_t attribs;
    
    
    float sizeRequest;
    int pixelSizeRequest;
    int dirty;
} topazLayout_t;


#define LAYOUT_TYPEID 0xfffffff9


static void topaz_layout__on_pre_step(topazEntity_t * ent, void * userData) {
    topazLayout_t * layout = userData;
    if (layout->dirty)
        topaz_layout_recalculate(ent);
}

static void topaz_layout__on_step(topazEntity_t * ent, void * userData) {
    topazLayout_t * layout = userData;
    layout->attribs.on_step(ent, layout->attribs.userData);    
}

static void topaz_layout__on_recalculate__horizontal(topazEntity_t * ent, void * userData) {
    topazLayout_t * layout = userData;
    
    topazLayout_Rectangle_t childSpace = layout->innerSpace;
    
    int totalHorizontalAvailable = childSpace.width;
    const topazArray_t * children = topaz_entity_get_children(ent);
    uint32_t len = topaz_array_get_size(children);
    int * empties = calloc(len, sizeof(int));
    int emptyCount = 0;
    float freeSpaceFraction = 1;

    
    // first iterate through and find pixelSizeRequests
    uint32_t i;
    
    for(i = 0; i < len; ++i) {
        topazEntity_t * child = topaz_array_at(children, topazEntity_t *, i);
        if (topaz_entity_get_type_id(child) != LAYOUT_TYPEID)
            continue;
        int p = topaz_layout_get_pixel_size_request(child);
        if (p > 0) 
            totalHorizontalAvailable -= p;

        float sizeRequest = topaz_layout_get_size_request(child);
        if (sizeRequest > 0)
            freeSpaceFraction -= sizeRequest;
        
        if (p <= 0 && sizeRequest <= 0) {
            empties[i] = 1;
            emptyCount += 1;
        }
        
    }                

    topazLayout_Rectangle_t childBox = childSpace;

    for(i = 0; i < len; ++i) {
        topazEntity_t * child = topaz_array_at(children, topazEntity_t *, i);
        if (topaz_entity_get_type_id(child) != LAYOUT_TYPEID)
            continue;
        int p = topaz_layout_get_pixel_size_request(child);
        if (p > 0) {
            childBox.width = p;
        } else {
            if (empties[i]) 
                childBox.width = ((1/emptyCount) * freeSpaceFraction * totalHorizontalAvailable);
            else
                childBox.width = (topaz_layout_get_size_request(child) * totalHorizontalAvailable);
        }
        
        topazVector_t * pos = topaz_entity_position(child);
        pos->x = childBox.x;
        pos->y = childBox.y;
        topaz_layout_resize(child, childBox.width, childBox.height);
        
        childBox.x += childBox.width;
    }
    free(empties);
}


static void topaz_layout__on_recalculate__vertical(topazEntity_t * ent, void * userData) {
    topazLayout_t * layout = userData;
    
    
    
    topazLayout_Rectangle_t childSpace = layout->innerSpace;
    
    int totalVerticalAvailable = childSpace.height;
    const topazArray_t * children = topaz_entity_get_children(ent);
    int * empties = calloc(topaz_array_get_size(children), sizeof(int));
    int emptyCount = 0;
    float freeSpaceFraction = 1;

    
    // first iterate through and find pixelSizeRequests
    uint32_t i;
    uint32_t len = topaz_array_get_size(children);
    for(i = 0; i < len; ++i) {
        topazEntity_t * child = topaz_array_at(children, topazEntity_t *, i);
        if (topaz_entity_get_type_id(child) != LAYOUT_TYPEID)
            continue;
        int p = topaz_layout_get_pixel_size_request(child);
        if (p > 0) 
            totalVerticalAvailable -= p;
        
        float sizeRequest = topaz_layout_get_size_request(child);
        if (sizeRequest > 0)
            freeSpaceFraction -= sizeRequest;
        
        if (p <= 0 && sizeRequest <= 0) {
            empties[i] = 1;
            emptyCount += 1;
        }
    }                

    topazLayout_Rectangle_t childBox = childSpace;

    for(i = 0; i < len; ++i) {
        topazEntity_t * child = topaz_array_at(children, topazEntity_t *, i);
        if (topaz_entity_get_type_id(child) != LAYOUT_TYPEID)
            continue;
        int p = topaz_layout_get_pixel_size_request(child);
        if (p > 0) {
            childBox.height = p;
        } else {
            if (empties[i]) 
                childBox.height = ((1/emptyCount) * freeSpaceFraction * totalVerticalAvailable);
            else
                childBox.height = (topaz_layout_get_size_request(child) * totalVerticalAvailable);
        }
        
        topazVector_t * pos = topaz_entity_position(child);
        pos->x = childBox.x;
        pos->y = childBox.y;
        topaz_layout_resize(child, childBox.width, childBox.height);
        
        childBox.y += childBox.height;
    }    
    free(empties);
}


topazEntity_t * topaz_layout_create(
    /// The topaz context.
    topaz_t * context,
    
    const topazLayout_Attributes_t * attribsLayout
) {
    topazLayout_t * layout = calloc(1, sizeof(topazLayout_t));
    topazEntity_Attributes_t attribs = {};
    attribs.userData = layout;
    attribs.on_pre_step = topaz_layout__on_pre_step;

    layout->attribs = *attribsLayout;
    
    if (layout->attribs.on_step)
        attribs.on_step = topaz_layout__on_step;
    
    
    topazEntity_t * ent = topaz_entity_create(
        context,
        &attribs
    );
    return ent;
}

topazEntity_t * topaz_layout_create_horizontal(
    /// The topaz context.
    topaz_t * context
) {
    topazLayout_Attributes_t attribs = {};
    attribs.on_recalculate = topaz_layout__on_recalculate__horizontal;
    return topaz_layout_create(context, &attribs);

}

topazEntity_t * topaz_layout_create_vertical(
    /// The topaz context.
    topaz_t * context
) {
    topazLayout_Attributes_t attribs = {};
    attribs.on_recalculate = topaz_layout__on_recalculate__vertical;
    return topaz_layout_create(context, &attribs);
}



topazEntity_t * topaz_layout_setup(
    topazEntity_t * layoutIn,
    const topazLayout_Sizing_t * sizing
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;


    if (sizing->margin > 0) {
        layout->marginOffset.x = sizing->margin;
        layout->marginOffset.y = sizing->margin;
        layout->marginOffset.width = -sizing->margin*2;
        layout->marginOffset.height = -sizing->margin*2;
    }
    
    if (sizing->padding > 0) {
        layout->paddingOffset.x = sizing->padding;
        layout->paddingOffset.y = sizing->padding;
        layout->paddingOffset.width = -sizing->padding*2;
        layout->paddingOffset.height = -sizing->padding*2;
    }

    layout->marginOffset.x      += sizing->marginOffset.x;
    layout->marginOffset.y      += sizing->marginOffset.y;
    layout->marginOffset.width  += sizing->marginOffset.width;
    layout->marginOffset.height += sizing->marginOffset.height;


    layout->paddingOffset.x      += sizing->paddingOffset.x;
    layout->paddingOffset.y      += sizing->paddingOffset.y;
    layout->paddingOffset.width  += sizing->paddingOffset.width;
    layout->paddingOffset.height += sizing->paddingOffset.height;
    
    layout->sizeRequest = sizing->sizeRequest;
    layout->pixelSizeRequest = sizing->pixelSizeRequest;

    layout->dirty = 1;
    return layoutIn;
}

void topaz_layout_resize(
    topazEntity_t * layoutIn,
    int width,
    int height
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    layout->bounds.width = width;
    layout->bounds.height = height;
    layout->dirty = 1;
}


void topaz_layout_recalculate(
    topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    topazVector_t g = topaz_entity_get_global_position(layoutIn);
    layout->bounds.x = g.x;
    layout->bounds.y = g.y;


    layout->outerSpace.x = layout->bounds.x + layout->marginOffset.x;
    layout->outerSpace.y = layout->bounds.y + layout->marginOffset.y;
    layout->outerSpace.width = layout->bounds.width + layout->marginOffset.width;
    layout->outerSpace.height = layout->bounds.height + layout->marginOffset.height;
    
    layout->innerSpace.x = layout->outerSpace.x + layout->paddingOffset.x;
    layout->innerSpace.y = layout->outerSpace.y + layout->paddingOffset.y;
    layout->innerSpace.width = layout->outerSpace.width + layout->paddingOffset.width;
    layout->innerSpace.height = layout->outerSpace.height + layout->paddingOffset.height;
    
    
    
    uint32_t i;
    const topazArray_t * children = topaz_entity_get_children(layoutIn);
    uint32_t len = topaz_array_get_size(children);
    for(i = 0; i < len; ++i) {
        topazEntity_t * child = topaz_array_at(children, topazEntity_t *, i);
        if (topaz_entity_get_type_id(child) != LAYOUT_TYPEID)
            continue;
        
        topaz_layout_request_recalculate(child);
    }                    

    if (layout->attribs.on_recalculate)
        layout->attribs.on_recalculate(
            layoutIn,
            layout->attribs.userData
        );
    layout->dirty = 0;   
}


void topaz_layout_set_pixel_size_request(
    topazEntity_t * layoutIn,
    int pixelSizeRequest
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    layout->pixelSizeRequest = pixelSizeRequest;
}

int topaz_layout_get_pixel_size_request(
    const topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    return layout->pixelSizeRequest;

}


void topaz_layout_set_size_request(
    topazEntity_t * layoutIn,
    float sizeRequest
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    layout->sizeRequest = sizeRequest;
}

float topaz_layout_get_size_request(
    const topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    return layout->sizeRequest;
}


void topaz_layout_set_margin_offset(
    topazEntity_t * layoutIn,
    const topazLayout_Rectangle_t * marginOffset
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    layout->marginOffset = *marginOffset;    
}

const topazLayout_Rectangle_t * topaz_layout_get_margin_offset(
    const topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    return &layout->marginOffset;    
}

void topaz_layout_set_padding_offset(
    topazEntity_t * layoutIn,
    const topazLayout_Rectangle_t * paddingOffset
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    layout->paddingOffset = *paddingOffset;
}

const topazLayout_Rectangle_t * topaz_layout_get_padding_offset(
    const topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    return &layout->paddingOffset;
}


const topazLayout_Rectangle_t * topaz_layout_get_child_space(
    const topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    return &layout->innerSpace;
}


const topazLayout_Rectangle_t * topaz_layout_get_content_space(
    const topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    return &layout->outerSpace;
}

void topaz_layout_request_recalculate(
    topazEntity_t * layoutIn
) {
    topazLayout_t * layout = topaz_entity_get_attributes(layoutIn)->userData;
    layout->dirty = 1;    
}

