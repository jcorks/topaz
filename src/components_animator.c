


#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__ANIMATOR = "t0p4zan1m4t0r";
#endif


typedef struct {
    // value at this keyframe 
    topazVector_t value;

    // The function to reach this 
    topazAnimator_Function func;

    // the offset in the animation
    float when;
} TAKeyframe;

typedef struct {
    // all keyframes
    topazArray_t * keyframes;

    // index of current keyframe in animation
    // the animation is always between previous 
    // keyframe and current. currentKeyframe - 1 should be 
    // used to interpolate.
    uint32_t currentKeyframe;

    // progress along the animation
    float progress;

    int paused;

    topaz_t * ctx;
    #ifdef TOPAZDC_DEBUG
        char * MAGIC_ID;
    #endif
} TopazAnimator;


static TopazAnimator * animator__retrieve(topazComponent_t * c) {
    TopazAnimator * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "TopazAnimator instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__ANIMATOR);
    #endif
    return s;
}



topazComponent_t * topaz_animator_create(
    /// The topaz context to create the component under.
    topaz_t * context

) {
    TopazAnimator * data = calloc(1, sizeof(TopazAnimator));
    data->ctx = t;
    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__SHAPE_2D;
    #endif

    data->keyframes = topaz_array_create(sizeof(TAKeyframe));
    data->currentKeyframe = 0;


    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_attach  = (topaz_component_attribute_callback) shape2d__on_attach;
    attribs.on_draw    = (topaz_component_attribute_callback) shape2d__on_draw;
    attribs.on_detach  = (topaz_component_attribute_callback) shape2d__on_detach;

    attribs.userData = data;
    topazComponent_t * out = topaz_component_create_with_attributes(TOPAZ_STR_CAST("Animator"), t, &attribs);
    return out;

}



void topaz_animator_add_keyframe(
    topazComponent_t * animator, 
    float value, 
    topazAnimator_Function lerp, 
    float animationOffset
) {
    TopazAnimator * a = animator__retrieve(animator);
    TAKeyframe frame;
    frame.value = *value;
    frame.func = lerp;
    uint32_t kSize = topaz_array_get_size(a->keyframes);
    if (kSize) {
        frame.when = topaz_array_at(a->keyframes, TAKeyframe, kSize-1).when + animationOffset;
    } else {
        frame.when = animationOffset
    }

    topaz_array_push(a->keyframes, frame);
}


void topaz_animator_clear(
    topazComponent_t * animator
) {
    TopazAnimator * a = animator__retrieve(animator);
    topaz_array_set_size(a->keyframes, 0);
}

void topaz_animator_add_animation(
    topazComponent_t * animator,
    topazComponent_t * other
) {
    TopazAnimator * a = animator__retrieve(animator);
    TopazAnimator * aOther = animator__retrieve(other);

    uint32_t i;
    uint32_t len = topaz_array_get_size(aOther->keyframes);
    for(i = 0; i < len; ++i) {
        topaz_array_push(
            a->keyframes,
            topaz_array_at(
                aOther->keyframes,
                TAKeyframe,
                i
            )
        );
    }
}

void topaz_animator_blend(
    topazComponent_t * animator,
    topazComponent_t * other
) {
    TopazAnimator * a = animator__retrieve(animator);
    TopazAnimator * aOther = animator__retrieve(other);


    uint32_t i, n;
    uint32_t aLen = topaz_array_get_size(a->keyframes);
    uint32_t otherLen = topaz_array_get_size(aOther->keyframes);

    TAKeyframe * otherFrames = topaz_array_get_data(aOther->keyframes)

    float progress = 0;
    TAKeyframe thisKey  = topaz_array_at(a->keyframes, TAKeyframe, i);
    TAKeyframe otherKey = topaz_array_at(a->keyframes, TAKeyframe, n);

    while(i < aLen && 
          n < otherLen) {


        if (otherFrames[n].when < thisKey.when) {
            // other precedent



            n++;
        } else {
            // this precedent
            if (o)

            progress = thisKey.value;
            i++;
        }
    }
}


