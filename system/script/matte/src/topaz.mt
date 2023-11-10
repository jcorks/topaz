@:MAPPED = {get::<-true};

::<= {
    // not used.
    @:topaz_script__run = getExternalFunction(name:'topaz_script__run');

}


@:initializer__viewport = ::<= {
    @:topaz_viewport__get_image = getExternalFunction(name:'topaz_viewport__get_image');
    @:topaz_viewport__resize = getExternalFunction(name:'topaz_viewport__resize');
    @:topaz_viewport__get_width = getExternalFunction(name:'topaz_viewport__get_width');
    @:topaz_viewport__get_height = getExternalFunction(name:'topaz_viewport__get_height');
    @:topaz_viewport__get_projection_3d = getExternalFunction(name:'topaz_viewport__get_projection_3d');
    @:topaz_viewport__set_filtered = getExternalFunction(name:'topaz_viewport__set_filtered');
    @:topaz_viewport__set_projection_3d_auto_mode = getExternalFunction(name:'topaz_viewport__set_projection_3d_auto_mode');
    @:topaz_viewport__set_projection_3d = getExternalFunction(name:'topaz_viewport__set_projection_3d');
    @:topaz_viewport__world_3d_to_screen = getExternalFunction(name:'topaz_viewport__world_3d_to_screen');
    @:topaz_viewport__screen_to_world_3d = getExternalFunction(name:'topaz_viewport__screen_to_world_3d');
    @:topaz_viewport__clear = getExternalFunction(name:'topaz_viewport__clear');
    @:topaz_viewport__set_auto_clear = getExternalFunction(name:'topaz_viewport__set_auto_clear');
    @:topaz_viewport__swap_buffers = getExternalFunction(name:'topaz_viewport__swap_buffers');
    @:topaz_viewport__sync = getExternalFunction(name:'topaz_viewport__sync');
    
    
    @:tempMatrix = {};
    @:tempMatrixSetter = ::(a, b, c, d, e, f, g, h, j, i , k , l, m ,n, o, p) {
        tempMatrix[0] = a;
        tempMatrix[1] = b;
        tempMatrix[2] = c;
        tempMatrix[3] = d;

        tempMatrix[4] = e;
        tempMatrix[5] = f;
        tempMatrix[6] = g;
        tempMatrix[7] = h;

        tempMatrix[8] = i;
        tempMatrix[9] = j;
        tempMatrix[10] = k;
        tempMatrix[11] = l;

        tempMatrix[12] = m;
        tempMatrix[13] = n;
        tempMatrix[14] = o;
        tempMatrix[15] = p;
    }
    
    
    @:temp = {};
    @:tempSetter = ::(a, b, c) {
        temp.x = a;
        temp.y = b;
        temp.z = c;
    }        
    @:getImage ::($) {
        @:out = topaz_viewport__get_image(a:$);
        if (out.__mapped == empty) ::<= {
            initializer__asset(a:out);
            initializer__image(i:out);      
        }
        return out;
    }
    
    
    @:resize ::($, width, height) {
        topaz_viewport__resize(a:$, b:width, c:height);
    }
    
    @:getWidth ::($) <- topaz_viewport__get_width(a:$)
    @:getHeight ::($) <- topaz_viewport__get_height(a:$)
    
    @:getProjection3d ::($) {
        topaz_viewport__get_projection_3d(a:$, b:tempMatrixSetter);
        return {...tempMatrix};
    }
    
    @:setFiltered ::($, enabled) {
        topaz_viewport__set_filtered(a:$, b:enabled);
    }
    
    @:setProjection3dAutoMode ::($, enabled) {
        topaz_viewport__set_projection_3d_auto_mode(a:$, b:enabled);
    }
    
    @:setProjection3d ::($, m) {
        topaz_viewport__set_projection_3d(a:$, b:m);
    }
    
    @:world3dToScreen ::($, point) {
        topaz_viewport__world_3d_to_screen(a:$, b:point.x, c:point.y, d:point.z, e:tempSetter);
        return {...temp};
    }
    
    @:screenToWorld3d ::($, point) {
        topaz_viewport__screen_to_world_3d(a:$, b:point.x, c:point.y, d:point.z, e:tempSetter);
        return {...temp};    
    }
    
    @:clear ::($) <- topaz_viewport__clear(a:$);
    
    @:setAutoClear ::($, enabled) {
        topaz_viewport__set_auto_clear(a:$, b:enabled);
    }
    
    @:swapBuffers ::($) <- topaz_viewport__swap_buffers(a:$);
    
    @:sync ::($) <- topaz_viewport__sync(a:$);
    
    
    return ::(v) {
        v.__mapped = MAPPED;

        v.getImage = getImage;
        v.resize = resize;
        v.getWidth = getWidth; 
        v.getHeight = getHeight;
        v.getProjection3d = getProjection3d;
        v.setFiltered = setFiltered;
        v.setProjection3dAutoMode = setProjection3dAutoMode;
        v.setProjection3d = setProjection3d;
        v.world3dToScreen = world3dToScreen;
        v.screenToWorld3d = screenToWorld3d;
        v.clear = clear;
        v.setAutoClear = setAutoClear;
        v.swapBuffers = swapBuffers;
        v.sync = v.sync;    
    }
}

@:initializer__text2d = ::<= {
    @:topaz_text2d__get_text = getExternalFunction(name:'topaz_text2d__get_text');
    @:topaz_text2d__set_text = getExternalFunction(name:'topaz_text2d__set_text');
    @:topaz_text2d__set_font = getExternalFunction(name:'topaz_text2d__set_font');
    @:topaz_text2d__set_color = getExternalFunction(name:'topaz_text2d__set_color');
    @:topaz_text2d__set_color_section = getExternalFunction(name:'topaz_text2d__set_color_section');
    @:topaz_text2d__get_extent_width = getExternalFunction(name:'topaz_text2d__get_extent_width');
    @:topaz_text2d__get_extent_height = getExternalFunction(name:'topaz_text2d__get_extent_height');
    @:topaz_text2d__get_char_x = getExternalFunction(name:'topaz_text2d__get_char_x');
    @:topaz_text2d__get_char_y = getExternalFunction(name:'topaz_text2d__get_char_y');
    @:topaz_text2d__get_position = getExternalFunction(name:'topaz_text2d__get_position');
    @:topaz_text2d__set_position = getExternalFunction(name:'topaz_text2d__set_position');
    @:topaz_text2d__set_scale = getExternalFunction(name:'topaz_text2d__set_scale');
    @:topaz_text2d__get_scale = getExternalFunction(name:'topaz_text2d__get_scale');
    @:topaz_text2d__set_rotation = getExternalFunction(name:'topaz_text2d__set_rotation');
    @:topaz_text2d__get_rotation = getExternalFunction(name:'topaz_text2d__get_rotation');
    @:topaz_text2d__set_attribute = getExternalFunction(name:'topaz_text2d__set_attribute');
    @:topaz_text2d__get_attribute = getExternalFunction(name:'topaz_text2d__get_attribute');

    @:setText ::($, text) {
        topaz_text2d__set_text(a:$, b:text);
    }
    
    @:setFont ::($, font, pixelSize) {
        topaz_text2d__set_font(a:$, b:font, c:pixelSize);
    }
    
    @:getText ::($) {
        return topaz_text2d__get_text(a:$);
    }
    
    @:getExtentWidth ::($) {
        return topaz_text2d__get_extent_width(a:$);
    }

    @:getExtentHeight ::($) {
        return topaz_text2d__get_extent_height(a:$);
    }
    
    @:getCharX ::($, index) {
        return topaz_text2d__get_char_x(a:$, b:index);
    }

    @:getCharY ::($, index) {
        return topaz_text2d__get_char_y(a:$, b:index);
    }

    @:temp = {};
    @:tempSetter = ::(a, b, c) {
        temp.x = a;
        temp.y = b;
        temp.z = c;
    }
    
    @:getPosition ::($) {
        topaz_text2d__get_position(a:$, b:tempSetter);
        return {...temp};
    }
    
    @:getRotation ::($) {
        topaz_text2d__get_rotation(a:$, b:tempSetter);
        return {...temp};
    }

    @:getScale ::($) {
        topaz_text2d__get_scale(a:$, b:tempSetter);
        return {...temp};
    }
    
    
    
    @:setPosition ::($, value) {
        topaz_text2d__set_position(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:setRotation ::($, value) {
        topaz_text2d__set_rotation(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:setScale ::($, value) {
        topaz_text2d__set_scale(a:$, b:value.x, c:value.y, d:value.z);
    }
    
    
    @:setColorSection ::($, fromIndex, toIndex, color) {
        topaz_text2d__set_color_section(a:$, b:fromIndex, c:toIndex, d:color);
    }
    
    @:setColor ::($, color) {
        topaz_text2d__set_color(a:$, b:color);
    }
    
    @:setAttribute ::($, attribute, value) {
        topaz_text2d__set_attribute(a:$, b:attribute, c:value);
    }
    
    @:getAttribute ::($, attribute) {
        return topaz_text2d__get_attribute(a:$, b:attribute);
    }

    return ::(t) {
        t.__mapped = MAPPED;
        
        t.setText = setText;
        t.setFont = setFont;
        t.getText = getText;
        t.getExtentWidth = getExtentWidth;
        t.getExtentHeight = getExtentHeight;
        t.getCharX = getCharX;
        t.getCharY = getCharY;
        t.getPosition = getPosition;
        t.getScale = getScale;
        t.getRotation = getRotation;
        t.setPosition = setPosition;
        t.setScale = setScale;
        t.setRotation = setRotation;
        t.setColorSection = setColorSection;
        t.setColor = setColor;
        t.setAttribute = setAttribute;
        t.getAttribute = getAttribute;
    }
}

@:initializer__state_control = ::<= {
    @:topaz_state_control__add = getExternalFunction(name:'topaz_state_control__add');
    @:topaz_state_control__remove = getExternalFunction(name:'topaz_state_control__remove');
    @:topaz_state_control__execute = getExternalFunction(name:'topaz_state_control__execute');
    @:topaz_state_control__halt = getExternalFunction(name:'topaz_state_control__halt');
    @:topaz_state_control__is_halted = getExternalFunction(name:'topaz_state_control__is_halted');
    @:topaz_state_control__get_current = getExternalFunction(name:'topaz_state_control__get_current');

    @:add ::($, name, state) {
        topaz_state_control__add(
            a:$,
            b:name,
            c:state.onStep,
            d:state.onDraw,
            e:state.onEnter,
            f:state.onLeave
        );
    }
    
    @:remove ::($, name) {
        topaz_state_control__remove(a:$, b:name);
    }
    
    @:execute ::($, name) {
        topaz_state_control__execute(a:$, b:name);
    }
    
    @:halt ::($) {
        topaz_state_control__halt(a:$);
    }
    
    @:isHalted ::($) {
        return topaz_state_control__is_halted(a:$);
    }
    
    @:getCurrent ::($) {
        return topaz_state_control__get_current(a:$);
    }

    return ::(s) {
        s.__mapped = MAPPED;
        
        s.add = add;
        s.remove = remove;
        s.execute = execute;
        s.halt = halt;
        s.isHalted = isHalted;
        s.getCurrent = getCurrent;
    }
}

@:initializer__sound = ::<= {
    @:topaz_sound__get_sample_count = getExternalFunction(name:'topaz_sound__get_sample_count');
    @:topaz_sound__get_nth_sample_left = getExternalFunction(name:'topaz_sound__get_nth_sample_left');
    @:topaz_sound__get_nth_sample_right = getExternalFunction(name:'topaz_sound__get_nth_sample_right');
    @:topaz_sound__set_samples = getExternalFunction(name:'topaz_sound__set_samples');
    @:topaz_sound__is_in_use = getExternalFunction(name:'topaz_sound__is_in_use');

    @:getSampleCount ::($) {
        return topaz_sound__get_sample_count(a:$);
    }
    
    @:getSamples ::($) {
        @output = [];
        for(0, topaz_sound__get_sample_count(a:$)) ::(i) {
            output->push(value:topaz_sound__get_nth_sample_left(a:$, b:i));
            output->push(value:topaz_sound__get_nth_sample_right(a:$, b:i));
        }
        return output;
    }
    
    @:setSamples ::($, samples) {
        topaz_sound__set_samples(a:$, b:samples);
    }
    
    @:isInUse ::($) {
        return topaz_sound__is_in_use(a:$);
    }
    
    return ::(s) {
        s.__mapped = MAPPED;
        
        s.getSampleCount = getSampleCount;
        s.getSamples = getSamples;
        s.setSamples = setSamples;
        s.isInUse = isInUse;
    }
}

@:initializer__shape3d = ::<= {
    @:topaz_shape3d__set_position = getExternalFunction(name:'topaz_shape3d__set_position');
    @:topaz_shape3d__get_position = getExternalFunction(name:'topaz_shape3d__get_position');
    @:topaz_shape3d__set_rotation = getExternalFunction(name:'topaz_shape3d__set_rotation');
    @:topaz_shape3d__get_rotation = getExternalFunction(name:'topaz_shape3d__get_rotation');
    @:topaz_shape3d__set_scale = getExternalFunction(name:'topaz_shape3d__set_scale');
    @:topaz_shape3d__get_scale = getExternalFunction(name:'topaz_shape3d__get_scale');
    @:topaz_shape3d__set_attribute = getExternalFunction(name:'topaz_shape3d__set_attribute');
    @:topaz_shape3d__get_attribute = getExternalFunction(name:'topaz_shape3d__get_attribute');
    @:topaz_shape3d__set_texture = getExternalFunction(name:'topaz_shape3d__set_texture');
    @:topaz_shape3d__set_mesh = getExternalFunction(name:'topaz_shape3d__set_mesh');
    @:topaz_shape3d__set_material = getExternalFunction(name:'topaz_shape3d__set_material');

    @:temp = {};
    @:tempSetter ::(a, b, c) {
        temp.x = a;
        temp.y = b;
        temp.z = c;
    }

    @:getPosition ::($) {
        topaz_shape3d__get_position(a:$, b:tempSetter);
        return {...temp};
    }
    
    @:setPosition ::($, value) {
        topaz_shape3d__set_position(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:getRotation ::($) {
        topaz_shape3d__get_rotation(a:$, b:tempSetter);
        return {...temp};
    }
    
    @:setRotation ::($, value) {
        topaz_shape3d__set_rotation(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:getScale ::($) {
        topaz_shape3d__get_scale(a:$, b:tempSetter);
        return {...temp};
    }
    
    @:setScale ::($, value) {
        topaz_shape3d__set_scale(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:setTexture ::($, slot, image) {
        topaz_shape3d__set_texture(a:$, b:slot, c:image);
    }
    
    
    @:setMesh ::($, mesh) {
        topaz_shape3d__set_mesh(a:$, b:mesh);
    }
    
    @:setMaterial ::($, material) {
        topaz_shape3d__set_material(a:$, b:material);
    }

    @:setAttribute ::($, attribute, value) {
        topaz_shape3d__set_attribute(a:$, b:attribute, c:value);
    }
    
    @:getAttribute ::($, attribute) {
        return topaz_shape3d__get_attribute(a:$, b:attribute);
    }
    
    
    


    return ::(s) {
        s.__mapped = MAPPED;
        
        s.setPosition = setPosition;
        s.getPosition = getPosition;
        s.setRotation = setRotation;
        s.getRotation = getRotation;
        s.setScale = setScale;
        s.getScale = getScale;
        
        s.setTexture = setTexture;
        s.setMesh = setMesh;
        s.setMaterial = setMaterial;
        s.setAttribute = setAttribute;
        s.getAttribute = getAttribute;
    }    
}

@:initializer__scheduler = ::<= {
    @:topaz_scheduler__pause = getExternalFunction(name:'topaz_scheduler__pause');
    @:topaz_scheduler__resume = getExternalFunction(name:'topaz_scheduler__resume');
    @:topaz_scheduler__reset = getExternalFunction(name:'topaz_scheduler__reset');
    @:topaz_scheduler__get_task_interval_remaining = getExternalFunction(name:'topaz_scheduler__get_task_interval_remaining');

    @:pause ::($) {
        topaz_scheduler__pause(a:$);
    }
    
    @:resume ::($) {
        topaz_scheduler__resume(a:$);
    }
    
    @:reset ::($) {
        topaz_scheduler__reset(a:$);
    }
    
    @:getTaskIntervalRemaining ::($) {
        return topaz_scheduler__get_task_interval_remaining(a:$);
    }

    return ::(s) {
        s.__mapped = MAPPED;
        
        s.pause = pause;
        s.resume = resume;
        s.reset = reset;
        s.getTaskIntervalRemaining = getTaskIntervalRemaining;
    }
}

@:initializer__rng = ::<= {
    @:topaz_rng__set_seed = getExternalFunction(name:'topaz_rng__set_seed');
    @:topaz_rng__next_int = getExternalFunction(name:'topaz_rng__next_int');
    @:topaz_rng__next_value = getExternalFunction(name:'topaz_rng__next_value');
    
    @:setSeed ::($, seed) {
        topaz_rng__set_seed(a:$, b:seed);
    }
    
    @:nextInt ::($) {
        return topaz_rng__next_int(a:$);
    }
    
    @:nextValue ::($) {
        return topaz_rng__next_value(a:$);
    }
    
    return ::(r) {
        r.__mapped = MAPPED;
        
        r.setSeed = setSeed;
        r.nextInt = nextInt;
        r.nextValue = nextValue;
    }
}

@:initializer__particle_emitter_2d = ::<= {
    @:topaz_particle_emitter_2d__set_particle = getExternalFunction(name:'topaz_particle_emitter_2d__set_particle');
    @:topaz_particle_emitter_2d__emit = getExternalFunction(name:'topaz_particle_emitter_2d__emit');

    @:setParticle ::($, particle) {
        topaz_particle_emitter_2d__set_particle(a:$, b:particle);
    }
    
    @:emit ::($, count) {
        topaz_particle_emitter_2d__emit(a:$, b:count);
    }

    return ::(p) {
        p.__mapped = MAPPED;
        
        p.setParticle = setParticle;
        p.emit = emit;
    }
}

@:initializer__particle = ::<= {
    @:topaz_particle__set_attribute = getExternalFunction(name:'topaz_particle__set_attribute');
    @:topaz_particle__get_attribute = getExternalFunction(name:'topaz_particle__get_attribute');
    @:topaz_particle__set_noise_min = getExternalFunction(name:'topaz_particle__set_noise_min');
    @:topaz_particle__set_noise_max = getExternalFunction(name:'topaz_particle__set_noise_max');
    @:topaz_particle__set_offset_min = getExternalFunction(name:'topaz_particle__set_offset_min');
    @:topaz_particle__set_offset_max = getExternalFunction(name:'topaz_particle__set_offset_max');
    @:topaz_particle__set_function = getExternalFunction(name:'topaz_particle__set_function');
    @:topaz_particle__to_string = getExternalFunction(name:'topaz_particle__to_string');
    @:topaz_particle__set_from_string = getExternalFunction(name:'topaz_particle__set_from_string');
    @:topaz_particle__set_image = getExternalFunction(name:'topaz_particle__set_image');

    @:setFromString ::($, string) {
        topaz_particle__set_from_string(a:$, b:string);
    }
    
    @:toString ::($) {
        return topaz_particle__to_string(a:$);
    }
    
    @:setAttribute ::($, attribute, value) {
        topaz_particle__set_attribute(a:$, b:attribute, c:value);
    }
    
    @:getAttribute::($, attribute) {
        return topaz_particle__get_attribute(a:$, b:attribute);
    }
    
    @:setImage::($, image) {
        topaz_particle__set_image(a:$, b:image);
    }
    
    @:setNoiseMin::($, prop, value) {
        topaz_particle__set_noise_min(a:$, b:prop, c:value);
    }
    
    @:setNoiseMax::($, prop, value) {
        topaz_particle__set_noise_max(a:$, b:prop, c:value);
    }

    @:setOffsetMax::($, prop, value) {
        topaz_particle__set_offset_max(a:$, b:prop, c:value);
    }

    @:setOffsetMin::($, prop, value) {
        topaz_particle__set_offset_min(a:$, b:prop, c:value);
    }
    
    @:setFunction::($, prop, animString) {
        topaz_particle__set_function(a:$, b:prop, c:animString);
    }    

    return ::(p) {
        p.__mapped = MAPPED;
        
        
        p.setFromString = setFromString;
        p.toString = toString;
        p.setAttribute = setAttribute;
        p.getAttribute = getAttribute;
        p.setImage = setImage;
        p.setNoiseMin = setNoiseMin;
        p.setNoiseMax = setNoiseMax;
        p.setOffsetMin = setOffsetMin;
        p.setOffsetMax = setOffsetMax;
        p.setFunction = setFunction;
    }
}

@:initializer__object2d = ::<= {

    @:topaz_object2d__add_velocity = getExternalFunction(name:'topaz_object2d__add_velocity');
    @:topaz_object2d__add_velocity_towards = getExternalFunction(name:'topaz_object2d__add_velocity_towards');
    @:topaz_object2d__set_velocity = getExternalFunction(name:'topaz_object2d__set_velocity');
    @:topaz_object2d__set_velocity_towards = getExternalFunction(name:'topaz_object2d__set_velocity_towards');
    @:topaz_object2d__get_friction_x = getExternalFunction(name:'topaz_object2d__get_friction_x');
    @:topaz_object2d__set_friction_x = getExternalFunction(name:'topaz_object2d__set_friction_x');
    @:topaz_object2d__get_friction_y = getExternalFunction(name:'topaz_object2d__get_friction_y');
    @:topaz_object2d__set_friction_y = getExternalFunction(name:'topaz_object2d__set_friction_y');
    @:topaz_object2d__get_direction = getExternalFunction(name:'topaz_object2d__get_direction');
    @:topaz_object2d__halt = getExternalFunction(name:'topaz_object2d__halt');
    @:topaz_object2d__reset_motion = getExternalFunction(name:'topaz_object2d__reset_motion');
    @:topaz_object2d__get_velocity_x = getExternalFunction(name:'topaz_object2d__get_velocity_x');
    @:topaz_object2d__get_velocity_y = getExternalFunction(name:'topaz_object2d__get_velocity_y');
    @:topaz_object2d__set_velocity_x = getExternalFunction(name:'topaz_object2d__set_velocity_x');
    @:topaz_object2d__set_velocity_y = getExternalFunction(name:'topaz_object2d__set_velocity_y');
    @:topaz_object2d__get_speed = getExternalFunction(name:'topaz_object2d__get_speed');
    @:topaz_object2d__set_speed = getExternalFunction(name:'topaz_object2d__set_speed');
    @:topaz_object2d__get_next_position = getExternalFunction(name:'topaz_object2d__get_next_position');
    @:topaz_object2d__get_last_position = getExternalFunction(name:'topaz_object2d__get_last_position');
    @:topaz_object2d__get_group = getExternalFunction(name:'topaz_object2d__get_group');
    @:topaz_object2d__set_group = getExternalFunction(name:'topaz_object2d__set_group');
    @:topaz_object2d__set_unit_lock = getExternalFunction(name:'topaz_object2d__set_unit_lock');
    @:topaz_object2d__set_collider = getExternalFunction(name:'topaz_object2d__set_collider');
    @:topaz_object2d__set_collider_radial = getExternalFunction(name:'topaz_object2d__set_collider_radial');
    @:topaz_object2d__set_collider_rectangle = getExternalFunction(name:'topaz_object2d__set_collider_rectangle');
    @:topaz_object2d__get_collider_len = getExternalFunction(name:'topaz_object2d__get_collider_len');
    @:topaz_object2d__get_last_collided = getExternalFunction(name:'topaz_object2d__get_last_collided');
    @:topaz_object2d__get_last_collided_position = getExternalFunction(name:'topaz_object2d__get_last_collided_position');
    @:topaz_object2d__get_collider_point_x = getExternalFunction(name:'topaz_object2d__get_collider_point_x');
    @:topaz_object2d__get_collider_point_y = getExternalFunction(name:'topaz_object2d__get_collider_point_y');
    
    
    @:addVelocity ::($, factor, direction) {
        topaz_object2d__add_velocity(a:$, b:factor, c:direction);
    }
    
    @:addVelocityTowards ::($, factor, point, directionOffset) {
        topaz_object2d__add_velocity_towards(a:$, b:factor, c:point.x, d:point.y, e:directionOffset);
    }
    
    @:setVelocity ::($, factor, direction) {
        topaz_object2d__set_velocity(a:$, b:factor, c:direction);
    }
    
    @:setVelocityTowards ::($, factor, point, directionOffset) {
        topaz_object2d__set_velocity_towards(a:$, b:factor, c:point.x, d:point.y, e:directionOffset);
    }
    
    @:setFrictionX ::($, amount) {
        topaz_object2d__set_friction_x(a:$, b:amount);
    }

    @:setFrictionY ::($, amount) {
        topaz_object2d__set_friction_y(a:$, b:amount);
    }

    @:getFrictionX ::($) {
        return topaz_object2d__get_friction_x(a:$);
    }

    @:getFrictionY ::($) {
        return topaz_object2d__get_friction_y(a:$);
    }
    
    @:getDirection ::($) {
        return topaz_object2d__get_direction(a:$);
    }
    
    @:halt ::($) {
        topaz_object2d__halt(a:$);
    }
    
    @:resetMotion ::($) {
        topaz_object2d__reset_motion(a:$);
    }
    
    @:getVelocityX ::($) {
        return topaz_object2d__get_velocity_x(a:$);
    }

    @:getVelocityY ::($) {
        return topaz_object2d__get_velocity_y(a:$);
    }

    @:setVelocityX ::($, velocity) {
        topaz_object2d__set_velocity_x(a:$, b:velocity);
    }

    @:setVelocityY ::($, velocity) {
        topaz_object2d__set_velocity_y(a:$, b:velocity);
    }
    
    @:getSpeed ::($) {
        return topaz_object2d__get_speed(a:$);
    }
    
    @:setSpeed ::($, speed) {
        topaz_object2d__set_speed(a:$, b:speed);
    }
    
    
    @:temp = {};
    @:tempSetter = ::(a, b, c) {
        temp.x = a;
        temp.y = b;
        temp.z = c;
    }
    @:getNextPosition ::($) {
        topaz_object2d__get_next_position(a:$, b:tempSetter);
        return {...temp};
    }
 
    @:getLastPosition ::($) {
        topaz_object2d__get_last_position(a:$, b:tempSetter);
        return {...temp};    
    }

    @:getGroup ::($) {
        return topaz_object2d__get_group(a:$);
    }
    
    @:setGroup ::($, group) {
        topaz_object2d__set_group(a:$, b:group);
    }
    
    @:setCollider ::($, points) {
        topaz_object2d__set_collider(a:$, b:points);
    }
    
    @:setColliderRadial ::($, radius, numSides) {
        topaz_object2d__set_collider_radial(a:$, b:radius, c:numSides);
    }
    
    @:setColliderRectangle ::($, width, height) {
        topaz_object2d__set_collider_rectangle(a:$, b:width, c:height);
    }
    
    @:getCollider ::($) {
        @_collider = [];
        @iter = 0;
        @len = topaz_object2d__get_collider_len(a:$);
        for(0, len)::(i) {
            _collider[iter] = topaz_object2d__get_collider_point_x(a:$, b:i); iter += 1;
            _collider[iter] = topaz_object2d__get_collider_point_y(a:$, b:i); iter += 1;
        }    
        return _collider;
    }
    
    @:getLastCollided ::($, where) {
        @:output = topaz_object2d__get_last_collided(a:$);
        if (output.__mapped == empty)
            initializer__entity(e:output);

        @:setter = ::(a, b, c) {
            where.x = a;
            where.y = b;
            where.z = c;
        }
        
        topaz_object2d__get_last_collided_position(a:$, b:setter);
        return output;
    }

    @:setUnitLock ::($, enable) {
        topaz_object2d__set_unit_lock(a:$, b:enable);
    }
    
 
    return ::(o) {
        o.__mapped = MAPPED;
        
        o.addVelocity = addVelocity;
        o.addVelocityTowards = addVelocityTowards;
        o.setVelocity = setVelocity;
        o.setVelocityTowards = setVelocityTowards;
        o.setFrictionX = setFrictionX;
        o.setFrictionY = setFrictionY;
        o.getFrictionX = getFrictionX;
        o.getFrictionY = getFrictionY;
        o.getDirection = getDirection;
        o.halt = halt;
        o.resetMotion = resetMotion;
        o.getVelocityX = getVelocityX;
        o.getVelocityY = getVelocityY;
        o.setVelocityX = setVelocityX;
        o.setVelocityY = setVelocityY;
        o.getSpeed = getSpeed;
        o.setSpeed = setSpeed;
        o.getNextPosition = getNextPosition;
        o.getLastPosition = getLastPosition;
        o.setGroup = setGroup;
        o.getGroup = getGroup;
        o.setCollider = setCollider;
        o.setColliderRadial = setColliderRadial;
        o.setColliderRectangle = setColliderRectangle;
        o.getCollider = getCollider;
        o.getLastCollided = getLastCollided;
        o.setUnitLock = setUnitLock;
    }   
}

@:initializer__mesh = ::<= {
    @:topaz_mesh__get_vertex_count = getExternalFunction(name:'topaz_mesh__get_vertex_count');
    @:topaz_mesh__set_vertex_count = getExternalFunction(name:'topaz_mesh__set_vertex_count');
    @:topaz_mesh__get_vertex = getExternalFunction(name:'topaz_mesh__get_vertex');
    @:topaz_mesh__set_vertex = getExternalFunction(name:'topaz_mesh__set_vertex');
    @:topaz_mesh__add_object = getExternalFunction(name:'topaz_mesh__add_object');
    @:topaz_mesh__remove_object = getExternalFunction(name:'topaz_mesh__remove_object');
    @:topaz_mesh__define_vertices = getExternalFunction(name:'topaz_mesh__define_vertices');
    @:topaz_mesh__get_object_count = getExternalFunction(name:'topaz_mesh__get_object_count');
    @:topaz_mesh__get_object_indices = getExternalFunction(name:'topaz_mesh__get_object_indices');
    @:topaz_mesh__set_object_indices = getExternalFunction(name:'topaz_mesh__set_object_indices');


    @:v = {};
    @:vsetter = ::(a, b, c,   d, e, f,  g, h,  i, j, k, l) {
        v[0] = a;
        v[1] = b;
        v[2] = c;

        v[3] = d;
        v[4] = e;
        v[5] = f;

        v[6] = g;
        v[7] = h;


        v[8] = i;
        v[9] = j;
        v[10] = k;
        v[11] = l;
    }


    @:setVertexCount ::($, newCount) {
        topaz_mesh__set_vertex_count(a:$, b:newCount);
    }

    @:defineVertices ::($, vertices) {
        topaz_mesh__define_vertices(a:$, b:vertices);
    }
    
    @:getVertex ::($, index) {
        topaz_mesh__get_vertex(a:$, b:index, c:vsetter);
        return {...v};
    }
    
    @:setVertex ::($, index, data) {
        topaz_mesh__set_vertex(a:$, b:index, c:data);
    }
    
    @:getVertexCount ::($) {
        return topaz_mesh__get_vertex_count(a:$);
    }

    @:addObject ::($) {
        return topaz_mesh__add_object(a:$);
    }
    
    @:setObjectIndices ::($, index, indices) {
        topaz_mesh__set_object_indices(a:$, b:index, c:indices);
    }

    @:getObjectIndices ::($, index) {
        @:output = [];
        @:outputSizer = ::(a){};
        @:outputGetter = ::(a) {
            output->push(value:a);
        }
        topaz_mesh__get_object_indices(a:$, b:index, c:outputSizer, d:outputGetter);
        return output;
    }
    
    @:removeObject ::($, index) {
        topaz_mesh__remove_object(a:$, b:index);
    }
    
    @:getObjectCount ::($) {
        return topaz_mesh__get_object_count(a:$);
    }

    return ::(m) {
        m.__mapped = MAPPED;    
        m.setVertexCount = setVertexCount;
        m.defineVertices = defineVertices;
        m.getVertex = getVertex;
        m.setVertex = setVertex;
        m.getVertexCount = m.getVertexCount;
        
        m.addObject = addObject;
        m.getObjectIndices = getObjectIndices;
        m.setObjectIndices = setObjectIndices;
        m.removeObject = removeObject;
        m.getObjectCount = getObjectCount;
        
    }
}


@:initializer__material = ::<= {

    @:topaz_material__set_program_data = getExternalFunction(name:'topaz_material__set_program_data');
    @:topaz_material__get_program_data = getExternalFunction(name:'topaz_material__get_program_data');
    @:topaz_material__clear_sources = getExternalFunction(name:'topaz_material__clear_sources');
    @:topaz_material__set_program_source = getExternalFunction(name:'topaz_material__set_program_source');

    @:getProgramData ::($) {
        @:output = [];
        for(0, 28) ::(i) {
            output[i] = topaz_material__get_program_data(a:$, b:i);
        }
        return output;
    }
    
    @:setProgramData ::($, data) {
        for(0, 28) ::(i) {
            topaz_material__set_program_data(a:$, b:i, c:data[i]);
        }
    }
    
    
    @:setProgramSource ::($, language, vertexShader, fragmentShader) {
        topaz_material__set_program_source(a:$, b:language, c:vertexShader, d:fragmentShader);
    }

    @:clearSources ::($) {
        topaz_material__clear_sources(a:$);
    }

    return ::(m) {
        m.__mapped = MAPPED;    
        m.getProgramData = getProgramData;
        m.setProgramData = setProgramData;
        m.setProgramSource = setProgramSource;
        m.clearSources = clearSources;
    }
}

@:initializer__image = ::<= {
    @:topaz_image__get_width = getExternalFunction(name:'topaz_image__get_width');
    @:topaz_image__get_height = getExternalFunction(name:'topaz_image__get_height');
    @:topaz_image__get_frame_count = getExternalFunction(name:'topaz_image__get_frame_count');
    @:topaz_image__resize = getExternalFunction(name:'topaz_image__resize');
    @:topaz_image__add_frame = getExternalFunction(name:'topaz_image__add_frame');
    @:topaz_image__remove_frame = getExternalFunction(name:'topaz_image__remove_frame');
    @:topaz_image__set_frame_rgba_data = getExternalFunction(name:'topaz_image__set_frame_rgba_data');
    @:topaz_image__get_frame_rgba_data = getExternalFunction(name:'topaz_image__get_frame_rgba_data');


    @:addFrame ::($) {
        return topaz_image__add_frame(a:$);
    }
    @:removeFrame ::($, frame) {
        topaz_image__remove_frame(a:$, b:frame);
    }
    
    @:getFrameCount ::($) {
        return topaz_image__get_frame_count(a:$);
    }

    @:getWidth ::($) {
        return topaz_image__get_width(a:$);
    }
    @:getHeight ::($) {
        return topaz_image__get_height(a:$);
    }
    @:resize ::($, width, height) {
        topaz_image__resize(a:$, b:width, c:height);
    }
    
    @:setFrameRGBAData ::($, frame, rgbaData) {
        topaz_image__set_frame_rgba_data(a:$, b:frame, c:rgbaData);
    }
    
    @:getFrameRGBAData ::($, frame) {
        @:output = [];
        @len;

        @:getLen ::(a) {
            len = a;
        }
        
        @:putColor ::(a, b, c, d) {
            output->push(value:a);
            output->push(value:b);
            output->push(value:c);
            output->push(value:d);
        }
        topaz_image__get_frame_rgba_data(a:$, b:frame, c:getLen, d:putColor);
        return output;
    }


    return ::(i) {
        i.__mapped = MAPPED;
        i.addFrame = addFrame;
        i.removeFrame = removeFrame;
        i.getFrameCount = getFrameCount;
        i.getWidth = getWidth;
        i.getHeight = getHeight;
        i.resize = resize;
        i.setFrameRGBAData = setFrameRGBAData;
        i.getFrameRGBAData = getFrameRGBAData;
    }
}

@:initializer__filesystem_path = ::<= {
    @:topaz_filesystem_path__as_string = getExternalFunction(name:'topaz_filesystem_path__as_string');
    @:topaz_filesystem_path__get_name = getExternalFunction(name:'topaz_filesystem_path__get_name');
    @:topaz_filesystem_path__get_parent = getExternalFunction(name:'topaz_filesystem_path__get_parent');
    @:topaz_filesystem_path__get_nth_child = getExternalFunction(name:'topaz_filesystem_path__get_nth_child');
    @:topaz_filesystem_path__get_child_count = getExternalFunction(name:'topaz_filesystem_path__get_child_count');

    @:asString ::($) {
        return topaz_filesystem_path__as_string(a:$);
    }
    
    @:getName ::($) {
        return topaz_filesystem_path__get_name(a:$);
    }
    
    @:getParent ::($) {
        @:out = topaz_filesystem_path__get_parent(a:$);
        if (out.__mapped == empty)
            initializer__filesystem_path(f:out);
        return out;
    }
    
    @:getChildren ::($) {
        @:output = [];
        for(0, topaz_filesystem_path__get_child_count(a:$)) ::(i) {
            output[i] = topaz_filesystem_path__get_nth_child(a:$, b:i);
            if (output[i].__mapped == empty)
                initializer__filesystem_path(f:output[i]);
        }
        return output;
    }

    return ::(f) {
        f.__mapped = MAPPED;
        f.asString = asString;
        f.getName = getName;
        f.getParent = getParent;
        f.getChildren = getChildren;
    }
}

@:initializer__command_context = ::<= {
    @:topaz_console__command_context_destroy = getExternalFunction(name:'topaz_console__command_context_destroy');
    @:topaz_console__command_context_add_command = getExternalFunction(name:'topaz_console__command_context_add_command');
    @:topaz_console__command_context_set_default_handler = getExternalFunction(name:'topaz_console__command_context_set_default_handler');
    @:topaz_console__command_context_set_prompt = getExternalFunction(name:'topaz_console__command_context_set_prompt');

    @:destroy = ::($) {
        topaz_console__command_context_destroy(a:$);
    }
    
    @:addCommand = ::($, command, callback) {
        @ args = [];
        @ argcount = 0;
        
        @:addArg::(a) {
            args[argcount] = a;  
            argcount+=1;
        }

        @:doAction::(a) {
            @:res = callback(
                args : args,
                fullCommand : a
            );
            args = [];
            argcount = 0;
            when(res == empty) '';
            return res;
        }   

        topaz_console__command_context_add_command(
            a:$,
            b:command,
            c:addArg,
            d:doAction
        )
    }
    
    @:setDefaultHandler = ::($, handler) {
        @ args = [];
        @ argcount = 0;
        
        @:addArg::(a) {
            args[argcount] = a;  
            argcount+=1;
        }

        @:doAction::(a) {
            @:res = handler(
                args : args,
                fullCommand : a
            );
            args = [];
            argcount = 0;
            when(res == empty) '';
            return res;
        }       
        
        topaz_console__command_context_set_default_handler(
            a:$,
            b:addArg,
            c:doAction
        );        
    }
    
    @:setPrompt = ::($, header) {
        topaz_console__command_context_set_prompt(a:$, b:header);
    }

    return ::(c) {
        c.__mapped = MAPPED;
        c.destroy = destroy;
        c.addCommand = addCommand;
        c.setDefaultHandler = setDefaultHandler;
        c.setPrompt = setPrompt;
    }
}


@:initializer__data = ::<= {
    @:topaz_data__get_byte_count = getExternalFunction(name:'topaz_data__get_byte_count');
    @:topaz_data__get_as_string = getExternalFunction(name:'topaz_data__get_as_string');
    @:topaz_data__get_as_base64 = getExternalFunction(name:'topaz_data__get_as_base64');
    @:topaz_data__get_nth_byte = getExternalFunction(name:'topaz_data__get_nth_byte');
    @:topaz_data__set_from_bytes = getExternalFunction(name:'topaz_data__set_from_bytes');
    @:topaz_data__set_from_string = getExternalFunction(name:'topaz_data__set_from_string');
    @:topaz_data__set_from_base64 = getExternalFunction(name:'topaz_data__set_from_base64');

    @:setFromBytes = ::($, bytes) {
        topaz_data__set_from_bytes(a:$, b:bytes);
    }
    
    @:setFromString = ::($, string) {
        topaz_data__set_from_string(a:$, b:string);
    }
    
    @:setFromBase64 = ::($, base64) {
        topaz_data__set_from_base64(a:$, b:base64);
    }
    
    
    
    @:getAsBytes = ::($) {
        @:bytes = [];
        for(0, topaz_data__get_byte_count(a:$)) ::(i) {
            bytes[i] = topaz_data__get_nth_byte(a:$, b:i);
        }
        return bytes;
    }
    
    @:getAsString = ::($) {
        return topaz_data__get_as_string(a:$);
    }
    
    @:getAsBase64 = ::($) {
        return topaz_data__get_as_base64(a:$);
    }


    return ::(d) {
        d.__mapped = MAPPED;
        d.setFromBytes = setFromBytes;
        d.setFromString = setFromString;
        d.setFromBase64 = setFromBase64;
        d.getAsBytes = getAsBytes;
        d.getAsString = getAsString;
        d.getAsBase64 = getAsBase64;
    }
}




@:initializer__automation = ::<= {

    @:topaz_automation__add_keyframe = getExternalFunction(name:'topaz_automation__add_keyframe');
    @:topaz_automation__add_vector_keyframe = getExternalFunction(name:'topaz_automation__add_vector_keyframe');
    @:topaz_automation__add_automation = getExternalFunction(name:'topaz_automation__add_automation');
    @:topaz_automation__clear = getExternalFunction(name:'topaz_automation__clear');
    @:topaz_automation__blend = getExternalFunction(name:'topaz_automation__blend');
    @:topaz_automation__smooth = getExternalFunction(name:'topaz_automation__smooth');
    @:topaz_automation__add_from_string = getExternalFunction(name:'topaz_automation__add_from_string');
    @:topaz_automation__skip_to = getExternalFunction(name:'topaz_automation__skip_to');
    @:topaz_automation__resume = getExternalFunction(name:'topaz_automation__resume');
    @:topaz_automation__pause = getExternalFunction(name:'topaz_automation__pause');
    @:topaz_automation__vector_at = getExternalFunction(name:'topaz_automation__vector_at');
    @:topaz_automation__at = getExternalFunction(name:'topaz_automation__at');
    @:topaz_automation__current_vector = getExternalFunction(name:'topaz_automation__current_vector');
    @:topaz_automation__current = getExternalFunction(name:'topaz_automation__current');
    @:topaz_automation__to_string = getExternalFunction(name:'topaz_automation__to_string');
    @:topaz_automation__set_from_string = getExternalFunction(name:'topaz_automation__set_from_string');
    @:topaz_automation__get_length = getExternalFunction(name:'topaz_automation__get_length');
    @:topaz_automation__set_duration_frames = getExternalFunction(name:'topaz_automation__set_duration_frames');
    @:topaz_automation__set_duration_seconds = getExternalFunction(name:'topaz_automation__set_duration_seconds');
    @:topaz_automation__get_duration = getExternalFunction(name:'topaz_automation__get_duration');
    @:topaz_automation__get_looped = getExternalFunction(name:'topaz_automation__get_looped');
    @:topaz_automation__set_looped = getExternalFunction(name:'topaz_automation__set_looped');
    @:topaz_automation__set_speed = getExternalFunction(name:'topaz_automation__set_speed');
    @:topaz_automation__get_speed = getExternalFunction(name:'topaz_automation__get_speed');

    
    @:addKeyframe ::($, value, function, offset) {
        topaz_automation__add_keyframe(a:$, b:value, c:function, d:offset);
    }
    
    @:addVectorKeyframe ::($, value, function, offset) {
        topaz_automation__add_vector_keyframe(a:$, b:value.x, c:value.y, d:value.z, e:function, f:offset);
    }
    
    @:clear ::($) {
        topaz_automation__clear(a:$);
    }
    
    @:addAutomation ::($, other) {
        topaz_automation__add_automation(a:$, b:other);
    }
    
    @:blend ::($, other) {
        topaz_automation__blend(a:$, b:other);
    }

    @:smooth ::($) {
        topaz_automation__smooth(a:$);
    }
    
    @:addFromString ::($, other) {
        topaz_automation__add_from_string(a:$, b:other);
    }
    
    @:getLength ::($) {
        return topaz_automation__get_length(a:$);
    }
    
    @:skipTo ::($, value) {
        topaz_automation__skip_to(a:$, b:value);
    }
    
    @:setDurationSeconds ::($, seconds) {
        topaz_automation__set_duration_seconds(a:$, b:seconds);
    }
    
    @:setDurationFrames ::($, frames) {
        topaz_automation__set_duration_frames(a:$, b:frames);
    }
    
    @:getDuration ::($) {
        return topaz_automation__get_duration(a:$);
    }

    @:setLooped ::($, loop) {
        topaz_automation__set_looped(a:$, b:loop);
    }

    @:getLooped ::($) {
        return topaz_automation__get_looped(a:$);
    }

    @:setSpeed ::($, speed) {
        topaz_automation__set_speed(a:$, b:speed);
    }

    @:getSpeed ::($) {
        return topaz_automation__get_speed(a:$);
    }
    
    @:pause ::($) {
        topaz_automation__pause(a:$);
    }

    @:resume ::($) {
        topaz_automation__resume(a:$);
    }
    
    @:toString ::($) {
        return topaz_automation__to_string(a:$);
    }
    
    @:setFromString ::($, automationString) {
        topaz_automation__set_from_string(a:$, b:automationString);
    }
    
    @:temp = {};
    @:tempSetter ::(a, b, c) {
        temp.x = a;
        temp.y = b;
        temp.c = c;
    }
    
    @:vectorAt ::($, progress) {
        topaz_automation__vector_at(a:$, b:progress, c:tempSetter);
        return {...temp};
    }   
    
    @:at ::($, progress) {
        return topaz_automation__at(a:$, b:progress);
    }
    
    @:currentVector ::($) {
        topaz_automation__current_vector(a:$, b:tempSetter);
        return {...temp};
    }
    
    @:current ::($) {
        return topaz_automation__current(a:$);
    }

    return ::(a) {
        a.__mapped = MAPPED;

        a.addKeyframe = addKeyframe;
        a.addVectorKeyframe = addVectorKeyframe;
        a.clear = clear;
        a.addAutomation = addAutomation;
        a.blend = blend;
        a.smooth = smooth;
        a.addFromString = addFromString;
        a.getLength = getLength;
        a.skipTo = skipTo;
        a.setDurationSeconds = setDurationSeconds;
        a.setDurationFrames = setDurationFrames;
        a.getDuration = getDuration;
        a.setLooped = setLooped;
        a.getLooped = getLooped;
        a.setSpeed = setSpeed;
        a.getSpeed = getSpeed;
        a.pause = pause;
        a.resume = resume;
        a.toString = toString;
        a.setFromString = setFromString;
        a.vectorAt = vectorAt;
        a.at = at;
        a.currentVector = currentVector;
        a.current = current;
    }
}



@:initializer__display = ::<= {
    @:topaz_display__get_viewport = getExternalFunction(name:'topaz_display__get_viewport');
    @:topaz_display__set_post_process_shader = getExternalFunction(name:'topaz_display__set_post_process_shader');
    @:topaz_display__set_parameter = getExternalFunction(name:'topaz_display__set_parameter');
    @:topaz_display__is_parameter_modifiable = getExternalFunction(name:'topaz_display__is_parameter_modifiable');
    @:topaz_display__get_parameter = getExternalFunction(name:'topaz_display__get_parameter');
    @:topaz_display__set_name = getExternalFunction(name:'topaz_display__set_name');
    @:topaz_display__add_parameter_callback = getExternalFunction(name:'topaz_display__add_parameter_callback');
    @:topaz_display__add_close_callback = getExternalFunction(name:'topaz_display__add_close_callback');
    @:topaz_display__remove_callback = getExternalFunction(name:'topaz_display__remove_callback');
    @:topaz_display__update = getExternalFunction(name:'topaz_display__update');

    @:getViewport ::($) {
        @:out = topaz_display__get_viewport(a:$);
        if (out.__mapped == empty) ::<= {
            initializer__entity(e:out);
            initializer__viewport(e:out);
        }
        return out;
    }



    
    
    @:setPostProcessShader ::($, vertexShader, fragmentShader) {
        topaz_display__set_post_process_shader(a:$, b:vertexShader, c:fragmentShader);
    }


    
    @:setParameter ::($, param, value) {
        topaz_display__set_parameter(a:$, b:param, c:value);
    }
    
    @:isParameterModifiable ::($, param) {
        return topaz_display__is_parameter_modifiable(a:$, b:param);
    }
    
    @:getParameter ::($, param) {
        return topaz_display__get_parameter(a:$, b:param);
    }

    
    @:setName ::($, name) {
        topaz_display__set_name(a:$, b:name);
    }

    @:addParameterCallback ::($, callback) {
        return topaz_display__add_parameter_callback(a:$, b:::(a, b) <- callback(display:a, parameter:b));
    }

    @:addCloseCallback ::($, callback) {
        return topaz_display__add_close_callback(a:$, b:::(a) <- callback(display:a));
    }
    
    @:removeCallback ::($, id) {
        topaz_display__remove_callback(a:$, b:id);
    }

    
    @:update ::($) {
        topaz_display__update(a:$);
    }

    return ::(d) {
        d.__mapped = MAPPED;
        
        d.getViewport = getViewport;
        d.setPostProcessShader = setPostProcessShader;
        d.setParameter = setParameter;
        d.isParameterModifiable = isParameterModifiable;
        d.getParameter = getParameter;
        d.setName = setName;
        d.addParameterCallback = addParameterCallback;
        d.addCloseCallback = addCloseCallback;
        d.removeCallback = removeCallback;
        d.update = update;
    }

}

@:initializer__asset = ::<= {
    @:topaz_asset__get_type = getExternalFunction(name:'topaz_asset__get_type');
    @:topaz_asset__get_name = getExternalFunction(name:'topaz_asset__get_name');

    @:getType = ::($) {
        return topaz_asset__get_type(a:$);
    }

    @:getName = ::($) {
        return topaz_asset__get_name(a:$);
    }

    
    return ::(a) {
        a.__mapped = MAPPED;
        a.getType = getType;          
        a.getName = getName;          
    }
}


@:initializer__entity = ::<= {

    @:topaz_entity__remove_component = getExternalFunction(name:'topaz_entity__remove_component');
    @:topaz_entity__add_component = getExternalFunction(name:'topaz_entity__add_component');

    @:topaz_entity__get_type_id = getExternalFunction(name:'topaz_entity__get_type_id');
    @:topaz_entity__set_type_id = getExternalFunction(name:'topaz_entity__set_type_id');
    

    @:topaz_entity__is_valid = getExternalFunction(name:'topaz_entity__is_valid');
    @:topaz_entity__remove = getExternalFunction(name:'topaz_entity__remove');
    @:topaz_entity__get_child_count = getExternalFunction(name:'topaz_entity__get_child_count');
    @:topaz_entity__get_nth_child = getExternalFunction(name:'topaz_entity__get_nth_child');
    @:topaz_entity__detach = getExternalFunction(name:'topaz_entity__detach');
    @:topaz_entity__attach = getExternalFunction(name:'topaz_entity__attach');
    @:topaz_entity__step = getExternalFunction(name:'topaz_entity__step');
    @:topaz_entity__draw = getExternalFunction(name:'topaz_entity__draw');
    @:topaz_entity__get_parent = getExternalFunction(name:'topaz_entity__get_parent');
    @:topaz_entity__query = getExternalFunction(name:'topaz_entity__query');
    @:topaz_entity__search = getExternalFunction(name:'topaz_entity__search');
    @:topaz_entity__get_priority = getExternalFunction(name:'topaz_entity__get_priority');
    @:topaz_entity__set_priority = getExternalFunction(name:'topaz_entity__set_priority');
    @:topaz_entity__set_priority_last = getExternalFunction(name:'topaz_entity__set_priority_last');
    @:topaz_entity__set_priority_first = getExternalFunction(name:'topaz_entity__set_priority_first');
    @:topaz_entity__get_rotation = getExternalFunction(name:'topaz_entity__get_rotation');
    @:topaz_entity__get_position = getExternalFunction(name:'topaz_entity__get_position');
    @:topaz_entity__get_scale = getExternalFunction(name:'topaz_entity__get_scale');
    @:topaz_entity__set_rotation = getExternalFunction(name:'topaz_entity__set_rotation');
    @:topaz_entity__set_position = getExternalFunction(name:'topaz_entity__set_position');
    @:topaz_entity__set_scale = getExternalFunction(name:'topaz_entity__set_scale');
    @:topaz_entity__look_at = getExternalFunction(name:'topaz_entity__look_at');
    @:topaz_entity__get_global_position = getExternalFunction(name:'topaz_entity__get_global_position');
    @:topaz_entity__is_stepping = getExternalFunction(name:'topaz_entity__is_stepping');
    @:topaz_entity__is_drawing = getExternalFunction(name:'topaz_entity__is_drawing');
    @:topaz_entity__get_drawing = getExternalFunction(name:'topaz_entity__get_drawing');
    @:topaz_entity__set_drawing = getExternalFunction(name:'topaz_entity__set_drawing');
    
    @:topaz_entity__get_local_matrix = getExternalFunction(name:'topaz_entity__get_local_matrix');
    @:topaz_entity__get_global_matrix = getExternalFunction(name:'topaz_entity__get_global_matrix');
    
    @:topaz_entity__get_stepping = getExternalFunction(name:'topaz_entity__get_stepping');
    @:topaz_entity__set_stepping = getExternalFunction(name:'topaz_entity__set_stepping');
    @:topaz_entity__get_name = getExternalFunction(name:'topaz_entity__get_name');
    @:topaz_entity__set_name = getExternalFunction(name:'topaz_entity__set_name');
    @:topaz_entity__add_component_after = getExternalFunction(name:'topaz_entity__add_component_after');
    @:topaz_entity__get_component_count = getExternalFunction(name:'topaz_entity__get_component_count');
    @:topaz_entity__get_nth_component = getExternalFunction(name:'topaz_entity__get_nth_component');
    @:topaz_entity__query_component = getExternalFunction(name:'topaz_entity__query_component');


    @:setTypeID = ::($, typeID) {
        topaz_entity__set_type_id(a:$, b:typeID);
    }
    
    @:getTypeID = ::($) {
        return topaz_entity__get_type_id(a:$);
    }
    
    @:isValid = ::($) {
        return topaz_entity__is_valid(a:$);
    }
    
    @:remove = ::($) {
        topaz_entity__remove(a:$);
    }
    
    @:getChildren = ::($) {
        @n = topaz_entity__get_child_count(a:$);
        @output = [];
        for(0, n) ::(i) {
            output[i] = topaz_entity__get_nth_child(a:$, b:i);
            if (output[i].__mapped == empty)
                initializer__entity(e:output[i])
        }
        return output;
    }
    
    @:step = ::($) {
        topaz_entity__step(a:$);
    }

    @:draw = ::($) {
        topaz_entity__draw(a:$);
    }
    
    @:attach = ::($, child) {
        topaz_entity__attach(a:$, b:child);
    }
    
    @:detach = ::($) {
        topaz_entity__detach(a:$);
    }
    
    @:getParent = ::($) {
        @:entity = topaz_entity__get_parent(a:$);
        if (entity.__mapped == empty)
            initializer__entity(entity);
        return entity;
    }
    
    @:query = ::($, name) {
        @:entity = topaz_entity__query(a:$, b:name);
        if (entity.__mapped == empty)
            initializer__entity(entity);
        return entity;
    }
    
    @:search = ::($, name) {
        @:entity = topaz_entity__search(a:$, b:name);
        if (entity.__mapped == empty)
            initializer__entity(entity);
        return entity;
    }
    
    @:setPriority = ::($, priority) {
        topaz_entity__set_priority(a:$, b:priority);
    }
    
    @:setPriorityLast = ::($) {
        topaz_entity__set_priority_last(a:$);
    }

    @:setPriorityFirst = ::($) {
        topaz_entity__set_priority_first(a:$);
    }

    @:getPriority = ::($) {
        topaz_entity__get_priority(a:$);
    }
    
    @:temp = {};
    @:tempSetter = ::(a, b, c) {
        temp.x = a;
        temp.y = b;
        temp.z = c;
    }
    
    @:getRotation = ::($) {
        topaz_entity__get_rotation(a:$, b:tempSetter);
        return {...temp};
    }

    @:getPosition = ::($) {
        topaz_entity__get_position(a:$, b:tempSetter);
        return {...temp};
    }

    @:getScale = ::($) {
        topaz_entity__get_scale(a:$, b:tempSetter);
        return {...temp};
    }
    
    @:setRotation = ::($, value) {
        topaz_entity__set_rotation(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:setPosition = ::($, value) {
        topaz_entity__set_position(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:setScale = ::($, value) {
        topaz_entity__set_scale(a:$, b:value.x, c:value.y, d:value.z);
    }
    
    @:lookAt = ::($, target, up) {
        topaz_entity__look_at(
            a:$,
            b:target.x, c:target.y, d:target.z,
            e:up.x, f:up.y, g:up.z
        );
    }
    
    @:getGlobalPosition = ::($) {
        topaz_entity__get_global_position(a:$, b:tempSetter);
        return {...temp};
    }

    @isStepping = ::($) {
        return topaz_entity__is_stepping(a:$);
    }

    @isDrawing = ::($) {
        return topaz_entity__is_drawing(a:$);
    }
    
    @setStepping = ::($, enable) {
        topaz_entity__set_stepping(a:$, b:enable);
    }

    @setDrawing = ::($, enable) {
        topaz_entity__set_drawing(a:$, b:enable);
    }

    @getStepping = ::($) {
        return topaz_entity__get_stepping(a:$);
    }

    @getDrawing = ::($) {
        return topaz_entity__get_drawing(a:$);
    }
    
    
    @:tempMatrix = {};
    @:tempMatrixSetter = ::(a, b, c, d, e, f, g, h, j, i , k , l, m ,n, o, p) {
        tempMatrix[0] = a;
        tempMatrix[1] = b;
        tempMatrix[2] = c;
        tempMatrix[3] = d;

        tempMatrix[4] = e;
        tempMatrix[5] = f;
        tempMatrix[6] = g;
        tempMatrix[7] = h;

        tempMatrix[8] = i;
        tempMatrix[9] = j;
        tempMatrix[10] = k;
        tempMatrix[11] = l;

        tempMatrix[12] = m;
        tempMatrix[13] = n;
        tempMatrix[14] = o;
        tempMatrix[15] = p;
    }
    
    @:getLocalMatrix = ::($) {
        topaz_entity__get_local_matrix(a:$, b:tempMatrixSetter);
        return {...tempMatrix};
    }

    @:getGlobalMatrix = ::($) {
        topaz_entity__get_global_matrix(a:$, b:tempMatrixSetter);
        return {...tempMatrix};
    }
    
    @:addComponent = ::($, component) {
        topaz_entity__add_component(a:$, b:component);
    };

    @:addComponentAfter = ::($, component) {
        topaz_entity__add_component_after(a:$, b:component);
    };
    
    @:getComponents = ::($) {
        @n = topaz_entity__get_component_count(a:$);
        @output = [];
        for(0, n) ::(i) {
            output[i] = topaz_entity__get_nth_component(a:$, b:i);
            if (output[i].__mapped == empty)
                initializer__component(c:output[i]);
            
        }
        return output;    
    }
    
    @:queryComponent = ::($, tag) {
        @:c = topaz_entity__query_component(a:$, b:tag);
        if (c.__mapped == empty)
            initializer__component(c);
        return c;

    }
    
    @:removeComponent = ::($, component) {
        topaz_entity__remove_component(a:$, b:component);
    }
    
    @:setName = ::($, newName) {
        topaz_entity__set_name(a:$, b:newName);
    }
    
    @:getName = ::($) {
        return topaz_entity__get_name(a:$);
    }

    
    return ::(e) {
        e.__mapped = MAPPED;

        e.setTypeID = setTypeID;
        e.getTypeID = getTypeID;
        e.isValid = isValid;
        e.remove = remove;
        e.getChildren = getChildren;
        e.step = step;
        e.draw = draw;
        e.attach = attach;
        e.detach = detach;
        e.getParent = getParent;
        e.query = query;
        e.search = search;
        e.setPriority = setPriority;
        e.setPriorityLast = setPriorityLast;
        e.setPriorityFirst = setPriorityFirst;
        e.getRotation = getRotation;
        e.getPosition = getPosition;
        e.getScale = getScale;
        e.setRotation = setRotation;
        e.setPosition = setPosition;
        e.setScale = setScale;
        e.lookAt = lookAt;
        e.getGlobalPosition = getGlobalPosition;
        e.isStepping = isStepping;
        e.isDrawing = isDrawing;
        e.setStepping = setStepping;
        e.setDrawing = setDrawing;
        e.getStepping = getStepping;
        e.getDrawing = getDrawing;
        e.getLocalMatrix = getLocalMatrix;
        e.getGlobalMatrix = getGlobalMatrix;
        e.addComponent = addComponent;
        e.addComponentAfter = addComponentAfter;
        e.getComponents = getComponents;
        e.queryComponent = queryComponent;
        e.removeComponent = removeComponent;
        e.setName = setName;
        e.getName = getName;
    }
}




@:initializer__component = ::<= {
    @:topaz_component__destroy = getExternalFunction(name:'topaz_component__destroy');
    @:topaz_component__detach = getExternalFunction(name:'topaz_component__detach');
    @:topaz_component__step = getExternalFunction(name:'topaz_component__step');
    @:topaz_component__draw = getExternalFunction(name:'topaz_component__draw');
    @:topaz_component__get_stepping = getExternalFunction(name:'topaz_component__get_stepping');
    @:topaz_component__set_stepping = getExternalFunction(name:'topaz_component__set_stepping');
    @:topaz_component__get_drawing = getExternalFunction(name:'topaz_component__get_drawing');
    @:topaz_component__set_drawing = getExternalFunction(name:'topaz_component__set_drawing');
    @:topaz_component__get_tag = getExternalFunction(name:'topaz_component__get_tag');
    @:topaz_component__get_host = getExternalFunction(name:'topaz_component__get_host');
    @:topaz_component__emit_event_anonymous = getExternalFunction(name:'topaz_component__emit_event_anonymous');
    @:topaz_component__emit_event = getExternalFunction(name:'topaz_component__emit_event');
    @:topaz_component__can_handle_event = getExternalFunction(name:'topaz_component__can_handle_event');
    @:topaz_component__install_event = getExternalFunction(name:'topaz_component__install_event');
    @:topaz_component__uninstall_event = getExternalFunction(name:'topaz_component__uninstall_event');
    @:topaz_component__install_hook = getExternalFunction(name:'topaz_component__install_hook');
    @:topaz_component__uninstall_hook = getExternalFunction(name:'topaz_component__uninstall_hook');
    @:topaz_component__install_handler = getExternalFunction(name:'topaz_component__install_handler');
    @:topaz_component__uninstall_handler = getExternalFunction(name:'topaz_component__uninstall_handler');


    @:destroy = ::($) {
        topaz_component__destroy(a:$);
    }

    @:detach = ::($) {
        topaz_component__detach(a:$);
    }

    @:step = ::($) {
        topaz_component__step(a:$);
    }
    
    @:draw = ::($) {
        topaz_component__draw(a:$);
    }
    
    @:getStepping = ::($) {
        return topaz_component__get_stepping(a:$);
    }
    
    @:getDrawing = ::($) {
        return topaz_component__get_drawing(a:$);
    }

    @:setStepping = ::($, enable) {
        topaz_component__set_stepping(a:$, b:enable);
    }

    @:setDrawing = ::($, enable) {
        topaz_component__set_drawing(a:$, b:enable);
    }
    
    @:getTag = ::($) {
        return topaz_component__get_tag(a:$);
    }
    
    @:getHost = ::($) {
        @:e = topaz_component__get_host(a:$);
        if (e.__mapped == empty)
            initializer__entity(e:e);
        return e;

    }
    
    @:emitEvent = ::($, event, source) {
        return topaz_component__emit_event(a:$, b:event, c:source);
    }
    
    @:emitEventAnonymous = ::($, event) {
        return topaz_component__emit_event_anonymous(a:$, b:event);
    }
    
    @:canHandleEvent = ::($, event) {
        return topaz_component__can_handle_event(a:$, b:event);
    }
    
    @:installHook = ::($, event, hook) {
        return topaz_component__install_hook(a:$, b:event, c:::(a, b) <- hook(component:a, source:b));
    }
    
    @:uninstallHook = ::($, event, id) {
        topaz_component__uninstall_hook(a:$, b:event, c:id);
    }

    @:installHandler = ::($, event, handler) {
        return topaz_component__install_handler(a:$, b:event, c:::(a, b) <- handler(component:a, source:b));
    }
    
    @:uninstallHandler = ::($, event, id) {
        topaz_component__uninstall_handler(a:$, b:event, c:id);
    }
    
    @:installEvent = ::($, event, defaultHandler) {
        topaz_component__install_event(a:$, b:event, c:::(a, b) <- defaultHandler(component:a, source:b));
    }
    
    @:uninstallEvent = ::($, event) {
        topaz_component__uninstall_event(a:$, b:event);
    }

    return ::(c) {
        c.__mapped = MAPPED;

    
        c.destroy = destroy;
        c.detach = detach;
        c.step = step;
        c.draw = draw;
        c.getStepping = getStepping;
        c.getDrawing = getDrawing;
        c.setStepping = setStepping;
        c.setDrawing = setDrawing;
        c.getTag = getTag;
        c.getHost = getHost;
        c.emitEvent = emitEvent;
        c.emitEventAnonymous = emitEventAnonymous;
        c.canHandleEvent = canHandleEvent;
        c.installHook = installHook;
        c.uninstallHook = uninstallHook;
        c.installHandler = installHandler;
        c.uninstallHandler = uninstallHandler;
        c.installEvent = installEvent;
        c.uninstallEvent = uninstallEvent;
    }
}


@:initializer__shape2d = ::<= {

    @:topaz_shape2d__get_color = getExternalFunction(name:'topaz_shape2d__get_color');
    @:topaz_shape2d__set_color = getExternalFunction(name:'topaz_shape2d__set_color');
    @:topaz_shape2d__get_anim_speed = getExternalFunction(name:'topaz_shape2d__get_anim_speed');
    @:topaz_shape2d__set_anim_speed = getExternalFunction(name:'topaz_shape2d__set_anim_speed');
    @:topaz_shape2d__get_center = getExternalFunction(name:'topaz_shape2d__get_center');
    @:topaz_shape2d__set_center = getExternalFunction(name:'topaz_shape2d__set_center');
    @:topaz_shape2d__set_position = getExternalFunction(name:'topaz_shape2d__set_position');
    @:topaz_shape2d__get_position = getExternalFunction(name:'topaz_shape2d__get_position');
    @:topaz_shape2d__set_rotation = getExternalFunction(name:'topaz_shape2d__set_rotation');
    @:topaz_shape2d__get_rotation = getExternalFunction(name:'topaz_shape2d__get_rotation');
    @:topaz_shape2d__set_scale = getExternalFunction(name:'topaz_shape2d__set_scale');
    @:topaz_shape2d__get_scale = getExternalFunction(name:'topaz_shape2d__get_scale');
    @:topaz_shape2d__form_lines = getExternalFunction(name:'topaz_shape2d__form_lines');
    @:topaz_shape2d__form_triangles = getExternalFunction(name:'topaz_shape2d__form_triangles');
    @:topaz_shape2d__set_attribute = getExternalFunction(name:'topaz_shape2d__set_attribute');
    @:topaz_shape2d__get_attribute = getExternalFunction(name:'topaz_shape2d__get_attribute');
    @:topaz_shape2d__form_rectangle = getExternalFunction(name:'topaz_shape2d__form_rectangle');
    @:topaz_shape2d__form_image = getExternalFunction(name:'topaz_shape2d__form_image');
    @:topaz_shape2d__form_image_frame = getExternalFunction(name:'topaz_shape2d__form_image_frame');
    @:topaz_shape2d__form_image_scaled = getExternalFunction(name:'topaz_shape2d__form_image_scaled');
    @:topaz_shape2d__form_image_frame_scaled = getExternalFunction(name:'topaz_shape2d__form_image_frame_scaled');
    @:topaz_shape2d__form_radial = getExternalFunction(name:'topaz_shape2d__form_radial');

    @:tempColor = {};
    @:tempColorSetter = ::(a, b, c, d) {
        tempColor.r = a;
        tempColor.g = b;
        tempColor.b = c;
        tempColor.a = d;
    }   

    @:getColor = ::($) {
        topaz_shape2d__get_color(a:$, b:tempColorSetter);
        return {...tempColor};
    }
    
    @:setColor = ::($, color) {
        topaz_shape2d__set_color(a:$, b:color.r, c:color.g, d:color.b, e:color.a);
    }
    
    @:setPosition = ::($, value) {
        topaz_shape2d__set_position(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:setScale = ::($, value) {
        topaz_shape2d__set_scale(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:setRotation = ::($, value) {
        topaz_shape2d__set_rotation(a:$, b:value.x, c:value.y, d:value.z);
    }

    @:temp = {};
    @:tempSetter = ::(a, b, c) {
        temp.x = a;
        temp.y = b;
        temp.z = c;
    }

    @:getPosition = ::($) {
        topaz_shape2d__get_position(a:$, b:tempSetter);
        return {...temp};
    }

    @:getScale = ::($) {
        topaz_shape2d__get_scale(a:$, b:tempSetter);
        return {...temp};
    }

    @:getRotation = ::($) {
        topaz_shape2d__get_rotation(a:$, b:tempSetter);
        return {...temp};
    }

    @:getAnimSpeed = ::($) {
        return topaz_shape2d__get_anim_speed(a:$);
    }

    @:setAnimSpeed = ::($, speed) {
        topaz_shape2d__set_anim_speed(a:$, b:speed);
    }
    
    @:setCenter = ::($, value) {
        topaz_shape2d__set_center(a:$, b:value.x, c:value.y, d:value.z);
    }    

    @:getCenter = ::($) {
        topaz_shape2d__get_center(a:$, b:tempSetter);
        return {...temp};
    }
    
    @:formRectangle = ::($, width, height) {
        topaz_shape2d__form_rectangle(a:$, b:width, c:height);
    }

    @:formImage = ::($, asset) {
        topaz_shape2d__form_image(a:$, b:asset);
    }

    @:formImageScaled = ::($, asset, width, height) {
        topaz_shape2d__form_image_scaled(a:$, b:asset, c:width, d:height);
    }
    
    @:formImageFrame = ::($, asset, frame) {
        topaz_shape2d__form_image_frame(a:$, b:asset, c:frame);
    }

    @:formImageFrameScaled = ::($, asset, frame, width, height) {
        topaz_shape2d__form_image_frame_scaled(a:$, b:asset, c:frame, d:width, e:height);
    }
    
    @:formRadial = ::($, radius, numIterations) {
        topaz_shape2d__form_radial(a:$, b:radius, c:numIterations);
    }
    
    @:formTriangles = ::($, vertices) {
        topaz_shape2d__form_triangles(a:$, b:vertices);
    }

    @:formLines = ::($, vertices) {
        topaz_shape2d__form_lines(a:$, b:vertices);
    }

    @:setAttribute = ::($, attribute, value) {
        topaz_shape2d__set_attribute(a:$, b:attribute, c:value);
    }

    @:getAttribute = ::($, attribute) {
        return topaz_shape2d__get_attribute(a:$, b:attribute);
    }


    return ::(c) {
        c.__mapped = MAPPED;
        c.getColor = getColor;
        c.setColor = setColor;
        c.setPosition = setPosition;
        c.setScale = setScale;
        c.setRotation = setRotation;
        c.getPosition = getPosition;
        c.getScale = getScale;
        c.getRotation = getRotation;
        c.getAnimSpeed = getAnimSpeed;
        c.setAnimSpeed = setAnimSpeed;
        c.setCenter = setCenter;
        c.getCenter = getCenter;
        c.formRectangle = formRectangle;
        c.formImage = formImage;
        c.formImageScaled = formImageScaled;
        c.formImageFrame = formImageFrame;
        c.formImageFrameScaled = formImageFrameScaled;
        c.formRadial = formRadial;
        c.formTriangles = formTriangles;
        c.formLines = formLines;
        c.getAttribute = getAttribute;
        c.setAttribute = setAttribute;
    }
}



@:Topaz = {
    Renderer : {
        TextureFilterHint : {
            Linear : 0,
            None : 1
        },
    
        Primitive : {
            Triangle : 0,
            Line : 1
        },
        
        DepthTest : {
            Less : 0,
            LEQ : 1,
            Greater : 2,
            GEQ : 3,
            Equal : 4,
            None : 5
        },
        
        AlphaRule : {
            Allow : 0,
            Opaque : 1,
            Translucent : 2,
            Invisible : 3
        },
        
        EtchRule : {
            NoEtching : 0,
            Define : 1,
            Undefine : 2,
            In : 3,
            Out : 4
        },
    
        Attribute : {
            Primitive : 0,
            AlphaRule : 1,
            DepthTest : 2,
            EtchRle : 3,
            TextureFilterHint : 4
        },
        
        DataLayer : {
            Color : 1,
            Depth : 2,
            Etch  : 4,
            All   : 7
        }
    },

    Asset : {
        'Type' : {
            None : 0,
            Image : 1,
            Sound : 2,
            Material : 3,
            Data : 4,
            Mesh : 5,
            Bundle : 6
        }
    },
    
    Entity : {
        create : ::<= {
            @:topaz_entity__create = getExternalFunction(name:'topaz_entity__create');

            return ::(attributes) {
                @:entOut = if (attributes != empty) 
                    topaz_entity__create(
                        a:attributes.onAttach,
                        b:attributes.onDetach,
                        c:attributes.onRemove,
                        d:attributes.onPreStep,
                        e:attributes.onStep,
                        f:attributes.onPreDraw,
                        g:attributes.onDraw
                    )              
                else 
                    topaz_entity__create()
                ;
                
                initializer__entity(e:entOut);
                return entOut;
            }
        }
    },
    
    Component : {
        create : ::<= {
            @:topaz_component__create = getExternalFunction(name:'topaz_component__create');
            
            
            return ::(tagName, attributes) {
                @:c = topaz_component__create(
                    a:tagName,
                    
                    b:attributes.onStep,
                    c:attributes.onDraw,
                    d:attributes.onAttach,
                    e:attributes.onDetach,
                    f:attributes.onDestroy
                );
                initializer__component(c:c);
                return c;
            }        
        }
    },
    
    Automation : {
        'Function' : {
            None : 0,
            Linear : 1,
            Accel : 2,
            Slow : 3,
            SoftAccel : 4,
            SoftSlow : 5,
            Random : 6
        },
        
        create : ::<= {
            @:topaz_automation__create = getExternalFunction(name:'topaz_automation__create');
            
            return :: {
                @:c = topaz_automation__create();
                initializer__component(c:c);
                initializer__automation(a:c);
                return c;                
            }
        }
    },
    
    Shape2D : {
        create : ::<= {
            @:topaz_shape2d__create = getExternalFunction(name:'topaz_shape2d__create');
            
            return :: {
                @:c = topaz_shape2d__create();
                initializer__component(c:c);
                initializer__shape2d(c);
                return c;
            }
        }
    },
    
    
    Audio : ::<= {
        @:topaz_audio__playback_set_volume = getExternalFunction(name:'topaz_audio__playback_set_volume');
        @:topaz_audio__playback_set_panning = getExternalFunction(name:'topaz_audio__playback_set_panning');
        @:topaz_audio__playback_set_repeat = getExternalFunction(name:'topaz_audio__playback_set_repeat');
        @:topaz_audio__playback_pause = getExternalFunction(name:'topaz_audio__playback_pause');
        @:topaz_audio__playback_resume = getExternalFunction(name:'topaz_audio__playback_resume');
        @:topaz_audio__playback_seek = getExternalFunction(name:'topaz_audio__playback_seek');
        @:topaz_audio__playback_stop = getExternalFunction(name:'topaz_audio__playback_stop');

        @:topaz_audio__play_sound = getExternalFunction(name:'topaz_audio__play_sound');
        @:topaz_audio__channel_halt = getExternalFunction(name:'topaz_audio__channel_halt');
        @:topaz_audio__channel_set_volume = getExternalFunction(name:'topaz_audio__channel_set_volume');
        @:topaz_audio__channel_set_panning = getExternalFunction(name:'topaz_audio__channel_set_panning');

        return {
            playSound ::(asset, channel) {
                return topaz_audio__play_sound(a:asset, b:channel);
            },
            
            Channel : {
                halt ::(channel) {
                    topaz_audio__channel_halt(a:channel);
                },
                
                setVolume ::(channel, amt) {
                    topaz_audio__channel_set_volume(a:channel, b:amt);
                },
                
                setPanning ::(channel, amt) {
                    topaz_audio__channel_set_panning(a:channel, b:amt);
                }
            },
            
            Playback : {
                setVolume ::(id, amt) {
                    topaz_audio__playback_set_volume(a:id, b:amt);
                },
                setPanning ::(id, amt) {
                    topaz_audio__playback_set_panning(a:id, b:amt);
                },
                setRepeat ::(id, repeat) {
                    topaz_audio__playback_set_repeat(a:id, b:repeat);                
                },
                seek ::(id, to) {
                    topaz_audio__playback_seek(a:id, b:to);                                
                },
                stop ::(id) {
                    topaz_audio__playback_stop(a:id);
                },
                pause ::(id) {
                    topaz_audio__playback_pause(a:id);                
                },
                resume ::(id) {
                    topaz_audio__playback_resume(a:id);                
                }
            }
        }    
    },
    
    ViewManager : ::<= {
        @:topaz_view_manager__create_display = getExternalFunction(name:'topaz_view_manager__create_display');
        @:topaz_view_manager__destroy_display = getExternalFunction(name:'topaz_view_manager__destroy_display');
        @:topaz_view_manager__get_default = getExternalFunction(name:'topaz_view_manager__get_default');
        @:topaz_view_manager__get_display_count = getExternalFunction(name:'topaz_view_manager__get_display_count');
        @:topaz_view_manager__get_display = getExternalFunction(name:'topaz_view_manager__get_display');
        @:topaz_view_manager__get_clipboard_as_string = getExternalFunction(name:'topaz_view_manager__get_clipboard_as_string');
        @:topaz_view_manager__set_clipboard_from_string = getExternalFunction(name:'topaz_view_manager__set_clipboard_from_string');
        
        return {
            createDisplay ::(displayName) {
                @:d = topaz_view_manager__create_display(a:displayName);
                initializer__display(d);
                return d;
            },
            
            destroyDisplay ::(display) {
                topaz_view_manager__destroy_display(a:display);
            },
            
            getDefault :: {
                @:d = topaz_view_manager__get_default();   
                if (d.__mapped == empty)
                    initializer__display(d);
                return d;
            },
            
            getClipboardAsString :: {
                return topaz_view_manager__get_clipboard_as_string();
            },
            
            setClipboardFromString ::(clipboardData) {
                topaz_view_manager__set_clipboard_from_string(a:clipboardData);
            },
            
            getAll :: {
                @:output = [];
                for(0, topaz_view_manager__get_display_count()) ::(i) {
                    output[i] = topaz_view_manager__get_display(a:i);
                    if (output[i].__mapped == empty)
                        initializer__display(d:output[i]);
                }
                return output;
            },
        }
    },
    
    Display : {
        ViewPolicy : {
            None      : 0,
            MatchSize : 1
        },

        Parameter : {
            X : 0,
            Y : 1,
            Width : 2,
            Height : 3,
            Show : 4,
            Fullscreen : 5,
            LockClientResize : 6,
            LockClientPosition : 7,
            ViewPolicy : 8,
            InputFocus : 9,
            Active : 10,
            Decorated:11
        }
    },

    Color : ::<= {
        @:topaz_color__to_hex_string = getExternalFunction(name:'topaz_color__to_hex_string');
        @:topaz_color__set_from_string = getExternalFunction(name:'topaz_color__set_from_string');

        @:colorTemp = {};
        @:colorTemp_mod ::(a, b, c, d) {
            colorTemp.r = a;
            colorTemp.g = b;
            colorTemp.b = c;
            colorTemp.a = d;
        }

        return {
            toString ::(color) {
                return topaz_color__to_hex_string(
                    a:color.r,
                    b:color.g,
                    c:color.b,
                    d:color.a
                );
            },
            
            fromString ::(str) {
                topaz_color__set_from_string(a:str, b:colorTemp_mod);
                return {...colorTemp};
            }
        }
    },
    
    Matrix : ::<= {
        @:topaz_matrix__set_identity = getExternalFunction(name:'topaz_matrix__set_identity');
        @:topaz_matrix__transform = getExternalFunction(name:'topaz_matrix__transform');
        @:topaz_matrix__to_string = getExternalFunction(name:'topaz_matrix__to_string');
        @:topaz_matrix__transpose = getExternalFunction(name:'topaz_matrix__transpose');
        @:topaz_matrix__invert = getExternalFunction(name:'topaz_matrix__invert');
        @:topaz_matrix__reverse_majority = getExternalFunction(name:'topaz_matrix__reverse_majority');
        @:topaz_matrix__multiply = getExternalFunction(name:'topaz_matrix__multiply');
        @:topaz_matrix__rotate_by_angles = getExternalFunction(name:'topaz_matrix__rotate_by_angles');
        @:topaz_matrix__rotate_by_angles = getExternalFunction(name:'topaz_matrix__rotate_by_angles');
        @:topaz_matrix__translate = getExternalFunction(name:'topaz_matrix__translate');
        @:topaz_matrix__scale = getExternalFunction(name:'topaz_matrix__scale');
        @:topaz_matrix__projection_perspective = getExternalFunction(name:'topaz_matrix__projection_perspective');
        @:topaz_matrix__projection_orthographic = getExternalFunction(name:'topaz_matrix__projection_orthographic');
        
        @:temp = {};
        @:tempSetter = ::(a, b, c) {
            temp.x = a;
            temp.y = b;
            temp.z = c;
        }        

        @:tempMatrix = {};
        @:tempMatrixSetter = ::(a, b, c, d, e, f, g, h, j, i , k , l, m ,n, o, p) {
            tempMatrix[0] = a;
            tempMatrix[1] = b;
            tempMatrix[2] = c;
            tempMatrix[3] = d;

            tempMatrix[4] = e;
            tempMatrix[5] = f;
            tempMatrix[6] = g;
            tempMatrix[7] = h;

            tempMatrix[8] = i;
            tempMatrix[9] = j;
            tempMatrix[10] = k;
            tempMatrix[11] = l;

            tempMatrix[12] = m;
            tempMatrix[13] = n;
            tempMatrix[14] = o;
            tempMatrix[15] = p;
        }
        
        @:matCopy ::(m) {
            for(0, 16) ::(i) {
                m[i] = tempMatrix[i];            
            }
        }
        
        return {
            setIdentity::(m) {
                topaz_matrix__set_identity(a:m, tempMatrixSetter);
                matCopy(m);
            },
            
            transform::(m, point) {
                topaz_matrix__transform(a:m, b:point.x, c:point.y, d:point.z, e:tempSetter);
                return {...temp};
            },
            
            toString ::(m) {
                return topaz_matrix__to_string(a:m);
            },
            
            transpose ::(m) {
                topaz_matrix__transpose(a:m, b:tempMatrixSetter);
                matCopy(m);
            },
            
            invert ::(m) {
                topaz_matrix__invert(a:m, b:tempMatrixSetter);
                matCopy(m);
            },
            
            reverseMajority ::(m) {
                topaz_matrix__reverse_majority(a:m, b:tempMatrixSetter);
                matCopy(m);
            },
            
            multiply ::(a, b) {
                topaz_matrix__multiply(a, b, c:tempMatrixSetter);
                return {...tempMatrix};
            },
            
            rotateByAngles ::(m, x, y, z) {
                topaz_matrix__rotate_by_angles(a:m, b:x, c:y, d:z, e:tempMatrixSetter);
                matCopy(m);
            },
            
            rotateByAxis ::(m, x, y, z, degrees) {
                topaz_matrix__rotate_by_axis(a:m, b:x, c:y, d:z, e:degrees, f:tempMatrixSetter);
                matCopy(m);
            },
            
            translate ::(m, x, y, z) {
                topaz_matrix__translate(a:m, b:x, c:y, d:z, e:tempMatrixSetter);
                matCopy(m);
            },
            
            scale ::(m, x, y, z) {
                topaz_matrix__scale(a:m, b:x, c:y, d:z, e:tempMatrixSetter);
                matCopy(m);
            },
            
            projectionPerspective(fovy, ratio, zNear, zFar) {
                topaz_matrix__projection_perspective(a:fovy, b:ratio, c:zNear, d:zFar, e:tempMatrixSetter);
                return {...tempMatrix};
            },
            
            projectionOrthographic(left, right, bottom, top, zNear, zFar) {
                topaz_matrix__projection_orthographic(a:left, b:right, c:bottom, d:top, e:zNear, f:zFar, g:tempMatrixSetter);
                return {...tempMatrix};
            }
        }
    },
    
    Vector : ::<= {
        @:topaz_vector__from_string = getExternalFunction(name:'topaz_vector__from_string');
        @:topaz_vector__reset = getExternalFunction(name:'topaz_vector__reset');
        @:topaz_vector__get_distance = getExternalFunction(name:'topaz_vector__get_distance');
        @:topaz_vector__normalize = getExternalFunction(name:'topaz_vector__normalize');
        @:topaz_vector__cross = getExternalFunction(name:'topaz_vector__cross');
        @:topaz_vector__reflect_2d = getExternalFunction(name:'topaz_vector__reflect_2d');
        @:topaz_vector__look_at_rotation = getExternalFunction(name:'topaz_vector__look_at_rotation');
        @:topaz_vector__point_at_2d = getExternalFunction(name:'topaz_vector__point_at_2d');
        @:topaz_vector__dot = getExternalFunction(name:'topaz_vector__dot');
        @:topaz_vector__floor = getExternalFunction(name:'topaz_vector__floor');
        @:topaz_vector__get_length = getExternalFunction(name:'topaz_vector__get_length');
        
        @:temp = {};
        @:tempSetter = ::(a, b, c) {
            temp.x = a;
            temp.y = b;
            temp.z = c;
        }
        
        
        return {
            fromString ::(input) {
                topaz_vector__from_string(a:input, b:tempSetter);
                return {...temp};
            },
            reset ::(v) {
                topaz_vector__reset(a:tempSetter);
                v.x = temp.x;
                v.y = temp.y;
                v.z = temp.z;
            },
            getLength ::(v) {
                return topaz_vector__get_length(a:v.x, b:v.y, c:v.z);
            },
            
            getDistance ::(p0, p1) {
                return topaz_vector__get_distance(
                    a:p0.x, b:p0.y, c:p0.z,
                    d:p1.x, e:p1.y, f:p1.z
                )
            },
            
            normalize ::(v) {
                topaz_vector__normalize(a:v.x, b:v.y, c:v.z, d:tempSetter);
                v.x = temp.x;
                v.y = temp.y;
                v.z = temp.z;
            },
            
            cross ::(a, b) {
                topaz_vector__cross(
                    a:a.x, b:a.y, c:a.z,
                    d:b.x, e:b.y, f:b.z,
                    g:tempSetter
                );
                return {...temp};                
            },
            
            dot ::(a, b) {
                topaz_vector__dot(
                    a:a.x, b:a.y, c:a.z,
                    a:b.x, b:b.y, c:b.z,
                    g:tempSetter
                );
                return {...temp};                
            },
            floor ::(v) {
                topaz_vector__floor(a:v.x, b:v.y, c:v.z, d:tempSetter);
                v.x = temp.x;
                v.y = temp.y;
                v.z = temp.z;
            },  
            
            reflect2d ::(direction, surface) {
                topaz_vector__reflect_2d(
                    a:direction.x, b:direction.y, c:direction.z,
                    d:surface.x, e:surface.y, f:surface.z,
                    g:tempSetter
                )
                return {...temp};
            },
            
            lookAtRotation ::(p0, p1, up) {
                topaz_vector__look_at_rotation(
                    a:p0.x, b:p0.y, c:p0.z,
                    d:p1.x, e:p1.y, f:p1.z,
                    g:up.x, h:up.y, i:up.z,
                    j:tempSetter
                );
                return {...temp};
            },
            
            
            pointAt2D ::(p0, p1) {
                return topaz_vector__point_at_2d(
                    a:p0.x, b:p0.y, c:p0.z,
                    d:p1.x, e:p1.y, f:p1.z
                )
            }
        }
    },
    
    Console : ::<= {
        @:topaz_console__enable = getExternalFunction(name:'topaz_console__enable');
        @:topaz_console__print = getExternalFunction(name:'topaz_console__print');
        @:topaz_console__print_message = getExternalFunction(name:'topaz_console__print_message');
        @:topaz_console__add_listener = getExternalFunction(name:'topaz_console__add_listener');
        @:topaz_console__remove_listener = getExternalFunction(name:'topaz_console__remove_listener');
        @:topaz_console__run = getExternalFunction(name:'topaz_console__run');
        @:topaz_console__push_command_context = getExternalFunction(name:'topaz_console__push_command_context');
        @:topaz_console__pop_command_context = getExternalFunction(name:'topaz_console__pop_command_context');
        @:topaz_console__command_context_create = getExternalFunction(name:'topaz_console__command_context_create');
        
        return {
            MessageType : {
                Normal : 0,
                Debug: 1,
                Warning: 2,
                Error : 3
            },
            enable ::(enable) {
                topaz_console__enable(a:enable);
            },
            
            'print' ::(message) {
                topaz_console__print(a:message);
            },
            
            printMessage ::(message, type) {
                topaz_console__print_message(a:message, b:type);
            },
            
            addListener ::(callback) {
                return topaz_console__add_listener(a:::(a, b) <- callback(line:a, type:b));
            },
            
            removeListener ::(id) {
                topaz_console__remove_listener(a:id);
            },
            
            run ::(command) {
                topaz_console__run(a:command);
            },
            
            CommandContext : {
                create :: {
                    @:out = topaz_console__command_context_create();
                    initializer__command_context(c:out);
                    return out;
                }
            },
            
            pushCommandContext ::(context) {
                topaz_console__push_command_context(a:context);
            },
            
            popCommandContext :: {
                topaz_console__pop_command_context();
            }
        }
    },
    
    Filesystem : ::<= {
        @:topaz_filesystem__get_path = getExternalFunction(name:'topaz_filesystem__get_path');
        @:topaz_filesystem__get_path_from_string = getExternalFunction(name:'topaz_filesystem__get_path_from_string');
        
        return {
            DefaultNode : {
                Resources : 0,
                Topaz : 1,
                UserData : 2,
                Bundles : 3
            },
            
            
            getPath ::(node) {
                @:out = topaz_filesystem__get_path(a:node);
                if (out.__mapped == empty)
                    initializer__filesystem_path(f:out);
                return out;
            },
            
            getPathFromString ::(from, path) {
                @:out = topaz_filesystem__get_path_from_string(a:from, b:path);
                if (out.__mapped == empty)
                    initializer__filesystem_path(f:out);
                return out;
            }
        }
    },
    
    FontManager : ::<= {
        @:topaz_font_manager__register_font = getExternalFunction(name:'topaz_font_manager__register_font');
        @:topaz_font_manager__preload_glyphs = getExternalFunction(name:'topaz_font_manager__preload_glyphs');
        @:topaz_font_manager__unregister_font = getExternalFunction(name:'topaz_font_manager__unregister_font');
        
        return {
            registerFont ::(asset) {
                return topaz_font_manager__register_font(a:asset);
            },
            
            preloadGlyphs ::(font, sizeRequest, characters) {
                topaz_font_manager__preload_glyphs(a:font, b:sizeRequest, c:characters);
            },
            
            unregisterFont ::(font) {
                topaz_font_manager__unregister_font(a:font);
            }
            
        }    
    },
    
    Input : ::<= {
        @:topaz_input__mouse_x = getExternalFunction(name:'topaz_input__mouse_x');
        @:topaz_input__mouse_y = getExternalFunction(name:'topaz_input__mouse_y');
        @:topaz_input__mouse_delta_x = getExternalFunction(name:'topaz_input__mouse_delta_x');
        @:topaz_input__mouse_delta_y = getExternalFunction(name:'topaz_input__mouse_delta_y');
        @:topaz_input__mouse_wheel = getExternalFunction(name:'topaz_input__mouse_wheel');
        @:topaz_input__add_keyboard_listener = getExternalFunction(name:'topaz_input__add_keyboard_listener');
        @:topaz_input__add_pad_listener = getExternalFunction(name:'topaz_input__add_pad_listener');
        @:topaz_input__add_pointer_listener = getExternalFunction(name:'topaz_input__add_pointer_listener');
        @:topaz_input__add_mapped_listener = getExternalFunction(name:'topaz_input__add_mapped_listener');
        @:topaz_input__remove_listener = getExternalFunction(name:'topaz_input__remove_listener');
        @:topaz_input__get_state = getExternalFunction(name:'topaz_input__get_state');
        @:topaz_input__get_pad_state = getExternalFunction(name:'topaz_input__get_pad_state');
        @:topaz_input__get_mapped_state = getExternalFunction(name:'topaz_input__get_mapped_state');
        @:topaz_input__set_deadzone = getExternalFunction(name:'topaz_input__set_deadzone');
        @:topaz_input__query_pad_count = getExternalFunction(name:'topaz_input__query_pad_count');
        @:topaz_input__query_pad_id = getExternalFunction(name:'topaz_input__query_pad_id');
        @:topaz_input__add_unicode_listener = getExternalFunction(name:'topaz_input__add_unicode_listener');
        @:topaz_input__remove_unicode_listener = getExternalFunction(name:'topaz_input__remove_unicode_listener');
        @:topaz_input__map = getExternalFunction(name:'topaz_input__map');    
        @:topaz_input__map_pad = getExternalFunction(name:'topaz_input__map_pad');    
        @:topaz_input__unmap = getExternalFunction(name:'topaz_input__unmap');    

        @:getListener ::(listener) {
            @:onPress   = listener.onPress;
            @:onActive  = listener.onActive;
            @:onRelease = listener.onRelease;
            @:onUpdate  = listener.onUpdate;
    
            return {
                onPress    : if(onPress)   ::(a)    <- onPress(input:a) else empty,
                onActive   : if(onActive)  ::(a, b) <- onActive(input:a, value:b) else empty,
                onRelease  : if(onRelease) ::(a)    <- onRelease(input:a) else empty,
                onUpdate   : if(onUpdate)  ::(a, b) <- onUpdate(input:a, value:b) else empty
            }
        }

        return {
            addKeyboardListener ::(listener) {
                return topaz_input__add_keyboard_listener(
                    a:getListener(listener)
                );
            },

            addPointerListener ::(listener) {
                return topaz_input__add_pointer_listener(
                    a:getListener(listener)
                );
            },

            addPadListener ::(listener, padIndex) {
                return topaz_input__add_pad_listener(
                    a:getListener(listener),
                    b:padIndex
                );
            },
            
            addMappedListener ::(listener, mappedName) {
                return topaz_input__add_mapped_listener(
                    a:getListener(listener),
                    b:mappedName
                );            
            },
            
            removeListener ::(id) {
                topaz_input__remove_listener(a:id);
            },
            
            getState ::(input) {
                return topaz_input__get_state(a:input);
            },
            
            getMappedState ::(input, name) {
                return topaz_input__get_mapped_state(a:input, b:name);
            },
            
            setDeadzone ::(padID, input, deadZone) {
                return topaz_input__set_deadzone(a:padID, b:input, c:deadZone);
            },
            
            queryPads ::{
                @:output = [];
                for(0, topaz_input__query_pad_count()) ::(i) {
                    output[i] = topaz_input__query_pad_id(i);
                }
                return output;
            },
            
            addUnicodeListener ::(listener) {
                @:onNewUnicode = listener.onNewUnicode;
                @:onRepeatUnicode = listener.onRepeatUnicode;
                
                return topaz_input__add_unicode_listener(
                    a: {
                        onNewUnicode    : if (onNewUnicode) ::(a) <- onNewUnicode(unicode:a) else empty,
                        onRepeatUnicode : if (onRepeatUnicode) ::(a) <- onRepeatUnicode(unicode:a) else empty 
                    }
                );
            },
            
            removeUnicodeListener ::(id) {
                topaz_input__remove_unicode_listener(a:id);
            },
            
            map ::(name, input) {
                topaz_input__map(a:name, b:input);
            },
            
            mapPad::(name, pad, input) {
                topaz_input__map_pad(a:name, b:pad, c:input);
            },
            unmap ::(name) {
                topaz_input__unmap(a:name);
            },
            
            mouseX :: {
                return topaz_input__mouse_x();
            },
            
            mouseY :: {
                return topaz_input__mouse_y();
            },

            mouseDeltaX :: {
                return topaz_input__mouse_delta_x();
            },    

            mouseDeltaY :: {
                return topaz_input__mouse_delta_y();
            },
            
            mouseWheel :: {
                return topaz_input__mouse_wheel();
            }
        }    
    },

    Key : {
        _0: 1, ///< 0
        _1: 2, ///< 1
        _2: 3, ///< 2
        _3: 4, ///< 3
        _4: 5, ///< 4
        _5: 6, ///< 5
        _6: 7, ///< 6
        _7: 8, ///< 7
        _8: 9, ///< 8
        _9: 10, ///< 9
        a: 11, ///< a
        b: 12, ///< b
        c: 13, ///< c
        d: 14, ///< d
        e: 15, ///< e
        f: 16, ///< f
        g: 17, ///< g
        h: 18, ///< h
        i: 19, ///< i
        j: 20, ///< j
        k: 21, ///< k
        l: 22, ///< l
        m: 23, ///< m
        n: 24, ///< n
        o: 25, ///< o
        p: 26, ///< p
        q: 27, ///< q
        r: 28, ///< r
        s: 29, ///< s
        t: 30, ///< t
        u: 31, ///< u
        v: 32, ///< v
        w: 33, ///< w
        x: 34, ///< x
        y: 35, ///< y
        z: 36, ///< z
        lshift: 37, ///< Left shift key
        rshift: 38, ///< Right shift key
        lctrl: 39,  ///< Left control key
        rctrl: 40,  ///< Right control key
        lalt: 41,   ///< Left alt key
        ralt: 42,   ///< Right alt key
        tab: 43,    ///< Tab
        F1: 44,     ///< F1
        F2: 45,     ///< F2
        F3: 46,     ///< F3
        F4: 47,     ///< F4
        F5: 48,     ///< F5
        F6: 49,     ///< F6
        F7: 50,     ///< F7
        F8: 51,     ///< F8
        F9: 52,     ///< F9
        F10: 53,    ///< F10
        F11: 54,    ///< F11
        F12: 55,    ///< F12
        up: 100,     ///< Up arrow
        down: 101,   ///< Down arrow
        left: 102,   ///< Left arrow
        right: 103,  ///< Right arrow
        minus: 104,  ///< -
        equal: 105,  ///< = 
        backspace: 106,  ///< Backspace
        grave: 107,  ///< `
        esc: 108,    ///< Escape
        home: 109,   ///< Home key
        pageUp: 110, ///< Page up key
        pageDown: 111,  ///< Page down key
        end: 112,    ///< End key
        backslash: 113, ///< '\'
        lbracket: 114, ///< [
        rbracket: 115, ///< ]
        semicolon: 116, ///< ;
        apostrophe: 117, ///< '
        frontslash: 118, ///< /
        enter: 119, ///< Enter
        delete: 120, ///< Delete
        numpad0: 121, ///< Numpad 0
        numpad1: 122, ///< Numpad 1
        numpad2: 123, ///< Numpad 2
        numpad3: 124, ///< Numpad 3
        numpad4: 125, ///< Numpad 4
        numpad5: 126, ///< Numpad 5
        numpad6: 127, ///< Numpad 6
        numpad7: 128, ///< Numpad 7
        numpad8: 129, ///< Numpad 8
        numpad9: 130, ///< Numpad 9
        prtscr: 131, ///< Print screen button
        lsuper: 132, ///< Left Super key (Windows key)
        rsuper: 133, ///< Right Super key (Windows key)
        space: 134,  ///< Space
        insert: 135, ///< Insert key
        comma: 136, ///< ,
        period: 137 , ///< .
        world1: 138, /// I8n key0
        world2: 139, /// I8n key1
        world3: 140, /// I8n key2
        world4: 141, /// I8n key3
        world5: 142, /// I8n key4
        world6: 143, /// I8n key5
        world7: 144, /// I8n key6
        world8: 145, /// I8n key7
        world9: 146 /// I8n key8
    },



    Pointer : {
        _0: 256, ///< Left click
        _1: 257, ///< Right click
        _2: 258, ///< Middle click
    
        X: 259, ///< Horizontal axis. Usually for the X axis of the pointer
        Y: 260, ///< Horizontal axis. Usually for the X axis of the pointer
        wheel: 261 ///< Mouse wheel.
    },

    Pad : {
    
        a: 300,     ///< Button 0
        b: 301,     ///< Button 1
        c: 302,     ///< Button 2
        x: 303,     ///< Button 3
        y: 304,     ///< Button 4
        r: 305,     ///< Button 5
        l: 306,     ///< Button 6
        r2: 307,    ///< Button 7
        l2: 308,    ///< Button 8
        r3: 309,    ///< Button 9
        l3: 310,    ///< Button 10
        start: 311,    ///< Button 11
        select: 312,///< Button 12
        b13: 313,///< Button 13
        d_up: 314,///< Button DPAD UP
        d_right: 315,///< Button DPAD RIGHT
        d_down: 316,///< Button DPAD DOWN
        d_left: 317,///< Button DPAD LEFT
        b18: 318,///< Button 18
        b19: 319,///< Button 19
        b20: 320,///< Button 20
        b21: 321,///< Button 21
        b22: 322,///< Button 22
        b23: 323,///< Button 23
        b24: 324,///< Button 24
        b25: 325,///< Button 25
        b26: 326,///< Button 26
        b27: 327,///< Button 27
        b28: 328,///< Button 28
        b29: 329,///< Button 29
        b30: 330,///< Button 30
        b31: 331,///< Button 31
        b32: 332,///< Button 32
    
        axisX: 400, ///< X button
        axisY: 401, ///< Y button
        axisZ: 402, ///< Z button
        axisX2: 403,///< X2 button 
        axisY2: 404,///< Y2 button
        axisZ2: 405,///< Z2 button
        axisX3: 406,///< X3 button
        axisY3: 407,///< Y3 button
        axisZ3: 408,///< Z3 button
        axisX4: 409,///< X4 button
        axisY4: 410,///< Y4 button
        axisZ4: 411,///< Z4 button
        axisX5: 412,///< X4 button
        axisY5: 413,///< Y4 button
        axisZ5: 414,///< Z4 button
        axisX6: 415,///< X4 button
        axisY6: 416,///< Y4 button
        axisZ6: 417,///< Z4 button
    
        axisR: 450,   
        axisL: 451,    
        axisR2: 452,    
        axisL2: 453,    
        axisR3: 454,    
        axisL3: 455,    
        axisR4: 456,    
        axisL4: 457,    
    
        options: 511
    },
    
    Object2D : ::<= {
        @:topaz_object2d__create = getExternalFunction(name:'topaz_object2d__create');
        @:topaz_object2d__set_group_interaction = getExternalFunction(name:'topaz_object2d__set_group_interaction');
    
        return {
            Group : {
                A : 0,
                B : 1,
                C : 2,
                D : 3,
                E : 4,
                F : 5,
                G : 6,
                H : 7,
                I : 8,
                J : 9,
                K : 10,
                L : 11,
                M : 12,
                N : 13,
                O : 14,
                P : 15,
                Q : 16,
                R : 17,
                S : 18,
                T : 19,
                U : 20,
                V : 21,
                W : 22,
                X : 23,
                Y : 24,
                Z : 25
            },
            
            create :: {
                @:out = topaz_object2d__create();
                initializer__object2d(o:out);
                initializer__component(c:out);
                return out;
            },
            
            setGroupInteraction ::(a, b, shouldInteract) {
                topaz_object2d__set_group_interaction(a, b, c:shouldInteract);
            }
        }
    },
    
    Particle : ::<= {
        @:topaz_particle__create = getExternalFunction(name:'topaz_particle__create');
        
        return {
            Property : {
                Duration : 0,
                ScaleX : 1,
                ScaleY : 2,
                ScaleMultiplier : 3,
                Rotation : 4,
                Direction : 5,
                SpeedX : 6,
                SpeedY : 7,
                Red : 8,
                Green : 9,
                Blue : 10,
                Alpha : 11
            },
            
            create ::{
                @:p = topaz_particle__create();
                initializer__particle(p);
                return p;
            }
        }
    },
    
    ParticleEmitter2D : ::<= {
        @:topaz_particle_emitter_2d__create = getExternalFunction(name:'topaz_particle_emitter_2d__create');
        return {
            create :: {
                @:out = topaz_particle_emitter_2d__create();
                initializer__particle_emitter_2d(p:out);
                initializer__entity(e:out);
                return out;
            }
        }        
    },
    
    Resources : ::<= {
        @:topaz_resources__fetch_asset = getExternalFunction(name:'topaz_resources__fetch_asset');
        @:topaz_resources__create_asset = getExternalFunction(name:'topaz_resources__create_asset');
        @:topaz_resources__create_data_asset_from_path = getExternalFunction(name:'topaz_resources__create_data_asset_from_path');
        @:topaz_resources__convert_asset = getExternalFunction(name:'topaz_resources__convert_asset');
        @:topaz_resources__write_asset = getExternalFunction(name:'topaz_resources__write_asset');
        @:topaz_resources__remove_asset = getExternalFunction(name:'topaz_resources__remove_asset');
        @:topaz_resources__is_extension_supported = getExternalFunction(name:'topaz_resources__is_extension_supported');
        @:topaz_resources__get_path = getExternalFunction(name:'topaz_resources__get_path');
        @:topaz_resources__set_path = getExternalFunction(name:'topaz_resources__set_path');
        @:topaz_resources__pack_bundle = getExternalFunction(name:'topaz_resources__pack_bundle');
        @:topaz_resources__unpack_bundle = getExternalFunction(name:'topaz_resources__unpack_bundle');
        @:topaz_resources__query_bundle = getExternalFunction(name:'topaz_resources__query_bundle');

        return {
            setPath ::(path) {
                return topaz_resources__set_path(a:path);
            },
            
            getPath :: {
                return topaz_resources__get_path();
            },
            
            createAsset ::(name, type) {
                @:out = topaz_resources__create_asset(a:name, b:type);
                when(out == empty) out;
                initializer__asset(a:out);
                @:typeO = Topaz.Asset.Type;
                match(type) {
                    (typeO.Image): initializer__image(i:out),
                    (typeO.Sound) : initializer__sound(s:out),
                    (typeO.Material) : initializer__material(m:out),
                    (typeO.Data) : initializer__data(d:out),
                    (typeO.Mesh) : initializer__mesh(m:out)
                }
                return out;
            },
            
            fetchAsset ::(name) {
                @:out = topaz_resources__fetch_asset(a:name);
                when(out == empty) out;
                when(out.__mapped) out;
                initializer__asset(a:out);
                @:typeO = Topaz.Asset.Type;
                match(out.getType()) {
                    (typeO.Image): initializer__image(i:out),
                    (typeO.Sound) : initializer__sound(s:out),
                    (typeO.Material) : initializer__material(m:out),
                    (typeO.Data) : initializer__data(d:out),
                    (typeO.Mesh) : initializer__mesh(m:out)
                }
                
                return out;                
            },
            
            createDataAssetFromPath ::(path, name) {
                @:out = topaz_resources__create_data_asset_from_path(a:path, b:name);
                when(out == empty) out;
                initializer__asset(a:out);
                initializer__data(d:out);
                return out;
            },
            
            convertAsset ::(fileType, asset) {
                @:out = topaz_resources__convert_asset(a:fileType, b:asset);
                when(out == empty) out;
                initializer__asset(a:out);
                @:typeO = Topaz.Asset.Type;
                match(out.getType()) {
                    (typeO.Image): initializer__image(i:out),
                    (typeO.Sound) : initializer__sound(s:out),
                    (typeO.Material) : initializer__material(m:out),
                    (typeO.Data) : initializer__data(d:out),
                    (typeO.Mesh) : initializer__mesh(m:out)
                }
                
                return out;                 
            },
            
            writeAsset ::(asset, fileType, outputPath) {
                return topaz_resources__write_asset(a:asset, b:fileType, c:outputPath);
            },
            
            removeAsset ::(asset) {
                topaz_resources__remove_asset(a:asset);
            },
            
            isExtensionSupported ::(name) {
                return topaz_resources__is_extension_supported(a:name);
            },
            
            packBundle ::(assetName, header, depends, assets) {
                @:out = topaz_resources__pack_bundle(a:assetName, b:header, c:depends, d:assets);
                when(out == empty) empty;
                initializer__asset(a:out);
                initializer__data(d:out);
                return out;
            },
            
            unpackBundle ::(bundleName, min_majorVersionRequired, min_minorVersionRequired, onNewItem) {
                return topaz_resources__unpack_bundle(a:bundleName,
                    b:min_majorVersionRequired,
                    c:min_minorVersionRequired,
                    d:onNewItem 
                );
            },
            
            queryBundle ::(bundleName) {
                return topaz_resources__query_bundle(a:bundleName);
            }
        }  
    },
    
    RNG : ::<= {
        @:topaz_rng__create = getExternalFunction(name:'topaz_rng__create');

        return {
            create :: {
                @:out = topaz_rng__create();
                initializer__rng(r:out);
                return out
            }
        }
    },
    
    Scheduler : ::<= {
        @:topaz_scheduler__create = getExternalFunction(name:'topaz_scheduler__create');
        return {
            Mode : {
                Time : 0,
                Frame : 1
            },
            create ::(
                mode,
                justOnce,
                interval,
                intervalDelay,
                callback
            ) {
                @:out = topaz_scheduler__create(a:mode, b:justOnce, c:interval, d:intervalDelay, e:callback);
                initializer__scheduler(s:out);
                initializer__component(c:out);
                return out;
            }
        }        
    },
    
    Shape3D : ::<= {
        @:topaz_shape3d__create = getExternalFunction(name:'topaz_shape3d__create');

        return { 
            create ::{ 
                @:out = topaz_shape3d__create();
                initializer__component(c:out);
                initializer__shape3d(s:out);
                return out;
            }
        }    
    },
    
    StateControl : ::<= {
        @:topaz_state_control__create = getExternalFunction(name:'topaz_state_control__create');

        return {
            create ::{
                @:out = topaz_state_control__create();
                initializer__component(c:out);
                initializer__state_control(c:out);
                return out;                
            }
        }   
    },
    
    Text2D : ::<= {
        @:topaz_text2d__create = getExternalFunction(name:'topaz_text2d__create');

        return {
            create ::{
                @:out = topaz_text2d__create();
                initializer__component(c:out);
                initializer__text2d(t:out);
                return out;
            }
        }
    },
    
    run   : getExternalFunction(name:'topaz__run'),
    pause   : getExternalFunction(name:'topaz__pause'),
    pauseNow : getExternalFunction(name:'topaz__pause_now'),
    resume : getExternalFunction(name:'topaz__resume'),
    iterate : getExternalFunction(name:'topaz__iterate'),
    isPaused : getExternalFunction(name:'topaz__is_paused'),
    debug : ::<= {
        @:debug = getExternalFunction(name:'topaz__debug')
        return ::{
            debug();
        }
    },
    quit : getExternalFunction(name:'topaz__quit'),
    getTime : getExternalFunction(name:'topaz__get_time'),
    getDeltaTime : getExternalFunction(name:'topaz__get_delta_time'),

    getVersionMinor : getExternalFunction(name:'topaz__get_version_minor'),
    getVersionMicro : getExternalFunction(name:'topaz__get_version_micro'),
    getVersionMajor: getExternalFunction(name:'topaz__get_version_major'),
    
    frameEnd : getExternalFunction(name:'topaz__frame_end')

}


::<= {
    @:topaz__attach_pre_manager = getExternalFunction(name:'topaz__attach_pre_manager');
    @:topaz__attach_pre_manager_unpausable = getExternalFunction(name:'topaz__attach_pre_manager_unpausable');
    @:topaz__attach_post_manager = getExternalFunction(name:'topaz__attach_post_manager');
    @:topaz__attach_post_manager_unpausable = getExternalFunction(name:'topaz__attach_post_manager_unpausable');
    @:topaz__frame_start = getExternalFunction(name:'topaz__frame_start');

    Topaz.attachPreManager = ::(entity) {
        topaz__attach_pre_manager(a:entity);
    }
    
    Topaz.attachPreManagerUnpausable = ::(entity) {
        topaz__attach_pre_manager_unpausable(a:entity);
    }

    Topaz.attachPostManager = ::(entity) {
        topaz__attach_post_manager(a:entity);
    }
    
    Topaz.attachPostManagerUnpausable = ::(entity) {
        topaz__attach_post_manager_unpausable(a:entity);
    }


    Topaz.frameStart = ::(FPS) {
        topaz__frame_start(a:FPS);
    }

};





return Topaz;
