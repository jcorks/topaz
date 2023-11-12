-- ALL input and output arrays are 1-indexed. It makes me sad!!!!!!!!!

Topaz = {};
(function() 

    local MAPPED = {}
    local copyVec = function(v) 
        local output = {};
        output.x = v.x;
        output.y = v.y;
        output.z = v.z;
        return output;
    end
    
    local copyMatrix = function(m) 
        local output = {};
        for i=1, 16 do 
            output[i] = m[i]
        end 
        return output;
    end
    
    local copyColor = function(c) 
        local output = {};
        output.r = c.r;
        output.g = c.g;
        output.b = c.b;
        output.a = c.a;
        return output;
    end

    local initializer__entity;
    local initializer__component;
    local initializer__asset;
    local initializer__image;
    initializer__entity = (function() 

        local setTypeID = function(REFi, typeID) 
            topaz_entity__set_type_id(REFi, typeID);
        end
        
        local getTypeID = function(REFi) 
            return topaz_entity__get_type_id(REFi);
        end
        
        local isValid = function(REFi) 
            return topaz_entity__is_valid(REFi);
        end
        
        local remove = function(REFi) 
            topaz_entity__remove(REFi);
        end
        
        local getChildren = function(REFi) 
            local n = topaz_entity__get_child_count(REFi);
            local output = {};
            for i=1, n do
                output[i] = topaz_entity__get_nth_child(REFi, i-1);
                if (output[i].__mapped == nil) then
                    initializer__entity(output[i])
                end
            end
            return output;
        end
        
        local step = function(REFi) 
            topaz_entity__step(REFi);
        end

        local draw = function(REFi) 
            topaz_entity__draw(REFi);
        end
        
        local attach = function(REFi, child) 
            topaz_entity__attach(REFi, child);
        end
        
        local detach = function(REFi) 
            topaz_entity__detach(REFi);
        end
        
        local getParent = function(REFi) 
            local entity = topaz_entity__get_parent(REFi);
            if (entity.__mapped == nil) then
                initializer__entity(entity);
            end
            return entity;
        end
        
        local query = function(REFi, name) 
            local entity = topaz_entity__query(REFi, name);
            if (entity.__mapped == nil) then
                initializer__entity(entity);
            end
            return entity;
        end
        
        local search = function(REFi, name) 
            local entity = topaz_entity__search(REFi, name);
            if (entity.__mapped == nil) then
                initializer__entity(entity);
            end
            return entity;
        end
        
        local setPriority = function(REFi, priority) 
            topaz_entity__set_priority(REFi, priority);
        end
        
        local setPriorityLast = function(REFi) 
            topaz_entity__set_priority_last(REFi);
        end

        local setPriorityFirst = function(REFi) 
            topaz_entity__set_priority_first(REFi);
        end

        local getPriority = function(REFi) 
            topaz_entity__get_priority(REFi);
        end
        
        local temp = {};
        local tempSetter = function(a, b, c) 
            temp.x = a;
            temp.y = b;
            temp.z = c;
        end
        
        local getRotation = function(REFi) 
            topaz_entity__get_rotation(REFi, tempSetter);
            return copyVec(temp);
        end

        local getPosition = function(REFi) 
            topaz_entity__get_position(REFi, tempSetter);
            return copyVec(temp);
        end

        local getScale = function(REFi) 
            topaz_entity__get_scale(REFi, tempSetter);
            return copyVec(temp);
        end
        
        local setRotation = function(REFi, value) 
            topaz_entity__set_rotation(REFi, value.x, value.y, value.z);
        end

        local setPosition = function(REFi, value) 
            topaz_entity__set_position(REFi, value.x, value.y, value.z);
        end

        local setScale = function(REFi, value) 
            topaz_entity__set_scale(REFi, value.x, value.y, value.z);
        end
        
        local lookAt = function(REFi, target, up) 
            topaz_entity__look_at(
                REFi,
                target.x, target.y, target.z,
                up.x, up.y, up.z
            );
        end
        
        local getGlobalPosition = function(REFi) 
            topaz_entity__get_global_position(REFi, tempSetter);
            return copyVec(temp);
        end

        local isStepping = function(REFi) 
            return topaz_entity__is_stepping(REFi);
        end

        local isDrawing = function(REFi) 
            return topaz_entity__is_drawing(REFi);
        end
        
        local setStepping = function(REFi, enable) 
            topaz_entity__set_stepping(REFi, enable);
        end

        local setDrawing = function(REFi, enable) 
            topaz_entity__set_drawing(REFi, enable);
        end

        local getStepping = function(REFi) 
            return topaz_entity__get_stepping(REFi);
        end

        local getDrawing = function(REFi) 
            return topaz_entity__get_drawing(REFi);
        end
        
        
        local tempMatrix = {};
        local tempMatrixSetter = function(a, b, c, d, e, f, g, h, j, i , k , l, m ,n, o, p) 
            tempMatrix[1] = a;
            tempMatrix[2] = b;
            tempMatrix[3] = c;
            tempMatrix[4] = d;

            tempMatrix[5] = e;
            tempMatrix[6] = f;
            tempMatrix[7] = g;
            tempMatrix[8] = h;

            tempMatrix[9] = i;
            tempMatrix[10] = j;
            tempMatrix[11] = k;
            tempMatrix[12] = l;

            tempMatrix[13] = m;
            tempMatrix[14] = n;
            tempMatrix[15] = o;
            tempMatrix[16] = p;
        end
        
        
        
        local getLocalMatrix = function(REFi) 
            topaz_entity__get_local_matrix(REFi, tempMatrixSetter);
            return copyMatrix(tempMatrix);
        end

        local getGlobalMatrix = function(REFi) 
            topaz_entity__get_global_matrix(REFi, tempMatrixSetter);
            return copyMatrix(tempMatrix);
        end
        
        local addComponent = function(REFi, component) 
            topaz_entity__add_component(REFi, component);
        end;

        local addComponentAfter = function(REFi, component) 
            topaz_entity__add_component_after(REFi, component);
        end;
        
        local getComponents = function(REFi) 
            local n = topaz_entity__get_component_count(REFi);
            local output = {};
            for i=1, n do 
                output[i] = topaz_entity__get_nth_component(REFi, i-1);
                if (output[i].__mapped == nil) then
                    initializer__component(output[i]);
                end
            end
            return output;    
        end
        
        local queryComponent = function(REFi, tag) 
            local c = topaz_entity__query_component(REFi, tag);
            if (c.__mapped == nil) then
                initializer__component(c);
            end
            return c;

        end
        
        local removeComponent = function(REFi, component) 
            topaz_entity__remove_component(REFi, component);
        end
        
        local setName = function(REFi, newName) 
            topaz_entity__set_name(REFi, newName);
        end
        
        local getName = function(REFi) 
            return topaz_entity__get_name(REFi);
        end

        
        return function(e) 
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
        end
    end)()
    local initializer__viewport = (function() 
        local tempMatrix = {};
        local tempMatrixSetter = function(a, b, c, d, e, f, g, h, j, i , k , l, m ,n, o, p) 
            tempMatrix[1] = a;
            tempMatrix[2] = b;
            tempMatrix[3] = c;
            tempMatrix[4] = d;

            tempMatrix[5] = e;
            tempMatrix[6] = f;
            tempMatrix[7] = g;
            tempMatrix[8] = h;

            tempMatrix[9] = i;
            tempMatrix[10] = j;
            tempMatrix[11] = k;
            tempMatrix[12] = l;

            tempMatrix[13] = m;
            tempMatrix[14] = n;
            tempMatrix[15] = o;
            tempMatrix[16] = p;
        end
        local temp = {};
        local tempSetter = function(a, b, c) 
            temp.x = a;
            temp.y = b;
            temp.z = c;
        end            
    
        local getImage = function(REFi) 
            local out = topaz_viewport__get_image(REFi);
            if (out.__mapped == nil) then
                initializer__asset(out);
                initializer__image(out);
            end            
            return out;
        end
        
        
        local resize = function(REFi, width, height)
            topaz_viewport__resize(REFi, width, height)
        end
        
        local getWidth = function(REFi)
            return topaz_viewport__get_width(REFi)
        end

        local getHeight = function(REFi)
            return topaz_viewport__get_height(REFi)
        end
        
        local getProjection3d = function(REFi)
            topaz_viewport__get_projection_3d(REFi, tempMatrixSetter);
            return copyMatrix(tempMatrix);
        end
        
        local setFiltered = function(REFi, enabled)
            topaz_viewport__set_filtered(REFi, enabled);
        end

        local setProjection3dAutoMode = function(REFi, enabled)
            topaz_viewport__set_projection_3d_auto_mode(REFi, enabled);
        end
        
        local setProjection3d = function(REFi, m)
            topaz_viewport__set_projection_3d(REFi, m);
        end
        
        local world3dToScreen = function(REFi, point)
            topaz_viewport__world_3d_to_screen(REFi, point.x, point.y, point.z, tempSetter);
            return copyVec(temp);
        end

        local screenToWorld3d = function(REFi, point)
            topaz_viewport__screen_to_world_3d(REFi, point.x, point.y, point.z, tempSetter);
            return copyVec(temp);
        end
        
        local clear = function(REFi, layer)
            topaz_viewport__screen_clear(REFi, layer);            
        end


        local setAutoClear = function(REFi, enable)
            topaz_viewport__set_auto_clear(REFi, enable);            
        end  

        local swapBuffers = function(REFi)
            topaz_viewport__swap_buffers(REFi);
        end

        local sync = function(REFi)
            topaz_viewport__sync(REFi);
        end      
        
        return function(v) 
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
        end
    end)()


    local initializer__text2d = (function() 
        local setText = function(REFi, text) 
            topaz_text2d__set_text(REFi, text);
        end
        
        local setFont = function(REFi, font, pixelSize) 
            topaz_text2d__set_font(REFi, font, pixelSize);
        end
        
        local getText = function(REFi) 
            return topaz_text2d__get_text(REFi);
        end
        
        local getExtentWidth = function(REFi) 
            return topaz_text2d__get_extent_width(REFi);
        end

        local getExtentHeight = function(REFi) 
            return topaz_text2d__get_extent_height(REFi);
        end
        
        local getCharX = function(REFi, index) 
            return topaz_text2d__get_char_x(REFi, index);
        end

        local getCharY = function(REFi, index) 
            return topaz_text2d__get_char_y(REFi, index);
        end

        local temp = {};
        local tempSetter = function(a, b, c) 
            temp.x = a;
            temp.y = b;
            temp.z = c;
        end
        
        local getPosition = function(REFi) 
            topaz_text2d__get_position(REFi, tempSetter);
            return copyVec(temp);
        end
        
        local getRotation = function(REFi) 
            topaz_text2d__get_rotation(REFi, tempSetter);
            return copyVec(temp);
        end

        local getScale = function(REFi) 
            topaz_text2d__get_scale(REFi, tempSetter);
            return copyVec(temp);
        end
        
        
        
        local setPosition = function(REFi, value) 
            topaz_text2d__set_position(REFi, value.x, value.y, value.z);
        end

        local setRotation = function(REFi, value) 
            topaz_text2d__set_rotation(REFi, value.x, value.y, value.z);
        end

        local setScale = function(REFi, value) 
            topaz_text2d__set_scale(REFi, value.x, value.y, value.z);
        end
        
        
        local setColorSection = function(REFi, fromIndex, toIndex, color) 
            topaz_text2d__set_color_section(REFi, fromIndex, toIndex, color);
        end
        
        local setColor = function(REFi, color) 
            topaz_text2d__set_color(REFi, color);
        end
        
        local setAttribute = function(REFi, attribute, value) 
            topaz_text2d__set_attribute(REFi, attribute, value);
        end
        
        local getAttribute = function(REFi, attribute) 
            return topaz_text2d__get_attribute(REFi, attribute);
        end

        return function(t) 
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
        end
    end)()

    local initializer__state_control = (function() 
        local add = function(REFi, name, state) 
            topaz_state_control__add(
                REFi,
                name,
                state.onStep,
                state.onDraw,
                state.onEnter,
                state.onLeave
            );
        end
        
        local remove = function(REFi, name) 
            topaz_state_control__remove(REFi, name);
        end
        
        local execute = function(REFi, name) 
            topaz_state_control__execute(REFi, name);
        end
        
        local halt = function(REFi) 
            topaz_state_control__halt(REFi);
        end
        
        local isHalted = function(REFi) 
            return topaz_state_control__is_halted(REFi);
        end
        
        local getCurrent = function(REFi) 
            return topaz_state_control__get_current(REFi);
        end

        return function(s) 
            s.__mapped = MAPPED;
            
            s.add = add;
            s.remove = remove;
            s.execute = execute;
            s.halt = halt;
            s.isHalted = isHalted;
            s.getCurrent = getCurrent;
        end
    end)()

    local initializer__sound = (function() 

        local getSampleCount = function(REFi) 
            return topaz_sound__get_sample_count(REFi);
        end
        
        local getSamples = function(REFi) 
            local output = {};
            for i=1, topaz_sound__get_sample_count(REFi) do
                output[i] = (topaz_sound__get_nth_sample_left(REFi, i-1));
                output[i] = (topaz_sound__get_nth_sample_right(REFi, i-1));
            end
            return output;
        end
        
        local setSamples = function(REFi, samples) 
            topaz_sound__set_samples(REFi, samples);
        end
        
        local isInUse = function(REFi) 
            return topaz_sound__is_in_use(REFi);
        end
        
        return function(s) 
            s.__mapped = MAPPED;
            
            s.getSampleCount = getSampleCount;
            s.getSamples = getSamples;
            s.setSamples = setSamples;
            s.isInUse = isInUse;
        end
    end)()

    local initializer__shape3d = (function() 

        local temp = {};
        local tempSetter = function(a, b, c) 
            temp.x = a;
            temp.y = b;
            temp.z = c;
        end

        local getPosition = function(REFi) 
            topaz_shape3d__get_position(REFi, tempSetter);
            return copyVec(temp);
        end
        
        local setPosition = function(REFi, value) 
            topaz_shape3d__set_position(REFi, value.x, value.y, value.z);
        end

        local getRotation = function(REFi) 
            topaz_shape3d__get_rotation(REFi, tempSetter);
            return copyVec(temp);
        end
        
        local setRotation = function(REFi, value) 
            topaz_shape3d__set_rotation(REFi, value.x, value.y, value.z);
        end

        local getScale = function(REFi) 
            topaz_shape3d__get_scale(REFi, tempSetter);
            return copyVec(temp);
        end
        
        local setScale = function(REFi, value) 
            topaz_shape3d__set_scale(REFi, value.x, value.y, value.z);
        end

        local setTexture = function(REFi, slot, image) 
            topaz_shape3d__set_texture(REFi, slot, image);
        end
        
        local setMesh = function(REFi, mesh) 
            topaz_shape3d__set_mesh(REFi, mesh);
        end
        
        local setMaterial = function(REFi, material) 
            topaz_shape3d__set_material(REFi, material);
        end

        local setAttribute = function(REFi, attribute, value) 
            topaz_shape3d__set_attribute(REFi, attribute, value);
        end
        
        local getAttribute = function(REFi, attribute) 
            return topaz_shape3d__get_attribute(REFi, attribute);
        end
        
        
        


        return function(s) 
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
        end    
    end)()

    local initializer__scheduler = (function() 
        local pause = function(REFi) 
            topaz_scheduler__pause(REFi);
        end
        
        local resume = function(REFi) 
            topaz_scheduler__resume(REFi);
        end
        
        local reset = function(REFi) 
            topaz_scheduler__reset(REFi);
        end
        
        local getTaskIntervalRemaining = function(REFi) 
            return topaz_scheduler__get_task_interval_remaining(REFi);
        end

        return function(s) 
            s.__mapped = MAPPED;
            
            s.pause = pause;
            s.resume = resume;
            s.reset = reset;
            s.getTaskIntervalRemaining = getTaskIntervalRemaining;
        end
    end)()

    local initializer__rng = (function()         
        local setSeed = function(REFi, seed) 
            topaz_rng__set_seed(REFi, seed);
        end
        
        local nextInt = function(REFi) 
            return topaz_rng__next_int(REFi);
        end
        
        local nextValue = function(REFi) 
            return topaz_rng__next_value(REFi);
        end
        
        return function(r) 
            r.__mapped = MAPPED;
            
            r.setSeed = setSeed;
            r.nextInt = nextInt;
            r.nextValue = nextValue;
        end
    end)()

    local initializer__particle_emitter_2d = (function() 

        local setParticle = function(REFi, particle) 
            topaz_particle_emitter_2d__set_particle(REFi, particle);
        end
        
        local emit = function(REFi, count) 
            topaz_particle_emitter_2d__emit(REFi, count);
        end

        return function(p) 
            p.__mapped = MAPPED;
            
            p.setParticle = setParticle;
            p.emit = emit;
        end
    end)()

    local initializer__particle = (function() 

        local setFromString = function(REFi, string) 
            topaz_particle__set_from_string(REFi, string);
        end
        
        local toString = function(REFi) 
            return topaz_particle__to_string(REFi);
        end
        
        local setAttribute = function(REFi, attribute, value) 
            topaz_particle__set_attribute(REFi, attribute, value);
        end
        
        local getAttribute = function(REFi, attribute) 
            return topaz_particle__get_attribute(REFi, attribute);
        end
        
        local setImage = function(REFi, image) 
            topaz_particle__set_image(REFi, image);
        end
        
        local setNoiseMin = function(REFi, prop, value) 
            topaz_particle__set_noise_min(REFi, prop, value);
        end
        
        local setNoiseMax = function(REFi, prop, value) 
            topaz_particle__set_noise_max(REFi, prop, value);
        end

        local setOffsetMax = function(REFi, prop, value) 
            topaz_particle__set_offset_max(REFi, prop, value);
        end

        local setOffsetMin = function(REFi, prop, value) 
            topaz_particle__set_offset_min(REFi, prop, value);
        end
        
        local setFunction = function(REFi, prop, animString) 
            topaz_particle__set_function(REFi, prop, animString);
        end    

        return function(p) 
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
        end
    end)()

    local initializer__object2d = (function() 

        local addVelocity = function(REFi, factor, direction) 
            topaz_object2d__add_velocity(REFi, factor, direction);
        end
        
        local addVelocityTowards = function(REFi, factor, point, directionOffset) 
            topaz_object2d__add_velocity_towards(REFi, factor, point.x, point.y, directionOffset);
        end
        
        local setVelocity = function(REFi, factor, direction) 
            topaz_object2d__set_velocity(REFi, factor, direction);
        end
        
        local setVelocityTowards = function(REFi, factor, point, directionOffset) 
            topaz_object2d__set_velocity_towards(REFi, factor, point.x, point.y, directionOffset);
        end
        
        local setFrictionX = function(REFi, amount) 
            topaz_object2d__set_friction_x(REFi, amount);
        end

        local setFrictionY = function(REFi, amount) 
            topaz_object2d__set_friction_y(REFi, amount);
        end

        local getFrictionX = function(REFi) 
            return topaz_object2d__get_friction_x(REFi);
        end

        local getFrictionY = function(REFi) 
            return topaz_object2d__get_friction_y(REFi);
        end
        
        local getDirection = function(REFi) 
            return topaz_object2d__get_direction(REFi);
        end
        
        local halt = function(REFi) 
            topaz_object2d__halt(REFi);
        end
        
        local resetMotion = function(REFi) 
            topaz_object2d__reset_motion(REFi);
        end
        
        local getVelocityX = function(REFi) 
            return topaz_object2d__get_velocity_x(REFi);
        end

        local getVelocityY = function(REFi) 
            return topaz_object2d__get_velocity_y(REFi);
        end

        local setVelocityX = function(REFi, velocity) 
            topaz_object2d__set_velocity_x(REFi, velocity);
        end

        local setVelocityY = function(REFi, velocity) 
            topaz_object2d__set_velocity_y(REFi, velocity);
        end
        
        local getSpeed = function(REFi) 
            return topaz_object2d__get_speed(REFi);
        end
        
        local setSpeed = function(REFi, speed) 
            topaz_object2d__set_speed(REFi, speed);
        end
        
        
        local temp = {};
        local tempSetter = function(a, b, c) 
            temp.x = a;
            temp.y = b;
            temp.z = c;
        end
        local getNextPosition = function(REFi) 
            topaz_object2d__get_next_position(REFi, tempSetter);
            return copyVec(temp);
        end
     
        local getLastPosition = function(REFi) 
            topaz_object2d__get_last_position(REFi, tempSetter);
            return copyVec(temp);    
        end

        local getGroup = function(REFi) 
            return topaz_object2d__get_group(REFi);
        end
        
        local setGroup = function(REFi, group) 
            topaz_object2d__set_group(REFi, group);
        end
        
        local setCollider = function(REFi, points) 
            topaz_object2d__set_collider(REFi, points);
        end
        
        local setColliderRadial = function(REFi, radius, numSides) 
            topaz_object2d__set_collider_radial(REFi, radius, numSides);
        end
        
        local setColliderRectangle = function(REFi, width, height) 
            topaz_object2d__set_collider_rectangle(REFi, width, height);
        end
        
        local getCollider = function(REFi) 
            local _collider = {};
            local iter = 1;
            local len = topaz_object2d__get_collider_len(REFi);
            for i=1,len do 
                _collider[iter] = topaz_object2d__get_collider_point_x(REFi, i-1); iter = iter + 1;
                _collider[iter] = topaz_object2d__get_collider_point_y(REFi, i-1); iter = iter + 1;
            end    
            return _collider;
        end
        
        local getLastCollided = function(REFi, where) 
            local output = topaz_object2d__get_last_collided(REFi);
            if (output.__mapped == nil) then
                initializer__entity(output);
            end

            local setter = function(a, b, c) 
                where.x = a;
                where.y = b;
                where.z = c;
            end
            
            topaz_object2d__get_last_collided_position(REFi, setter);
            return output;
        end

        local setUnitLock = function(REFi, enable) 
            topaz_object2d__set_unit_lock(REFi, enable);
        end
        
     
        return function(o) 
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
        end   
    end)()

    local initializer__mesh = (function() 

        local v = {};
        local vsetter = function(a, b, c,   d, e, f,  g, h,  i, j, k, l) 
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
        end

        local copyVertex = function(vtx) 
            local output = {};
            for i=1,12 do 
                output[i] = vtx[i];
            end            
            return output;
        end

        local setVertexCount = function(REFi, newCount) 
            topaz_mesh__set_vertex_count(REFi, newCount);
        end

        local defineVertices = function(REFi, vertices) 
            topaz_mesh__define_vertices(REFi, vertices);
        end
        
        local getVertex = function(REFi, index) 
            topaz_mesh__get_vertex(REFi, index, vsetter);
            return copyVertex(v);
        end
        
        local setVertex = function(REFi, index, data) 
            topaz_mesh__set_vertex(REFi, index, data);
        end
        
        local getVertexCount = function(REFi) 
            return topaz_mesh__get_vertex_count(REFi);
        end

        local addObject = function(REFi) 
            return topaz_mesh__add_object(REFi);
        end
        
        local setObjectIndices = function(REFi, index, indices) 
            topaz_mesh__set_object_indices(REFi, index, indices);
        end

        local getObjectIndices = function(REFi, index) 
            local output = {};
            local outputSizer = function(a) end;
            local iter = 0;
            local outputGetter = function(a) 
                output[iter] = a;
                iter = iter + 1;
            end
            topaz_mesh__get_object_indices(REFi, index, outputSizer, outputGetter);
            return output;
        end
        
        local removeObject = function(REFi, index) 
            topaz_mesh__remove_object(REFi, index);
        end
        
        local getObjectCount = function(REFi) 
            return topaz_mesh__get_object_count(REFi);
        end

        return function(m) 
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
            
        end
    end)()


    local initializer__material = (function() 

        local getProgramData = function(REFi) 
            local output = {};
            for i=1, 28 do 
                output[i] = topaz_material__get_program_data(REFi, i-1);
            end
            return output;
        end
        
        local setProgramData = function(REFi, data) 
            for i=1, 28 do 
                topaz_material__set_program_data(REFi, i-1, data[i]);
            end
        end
        
        
        local setProgramSource = function(REFi, language, vertexShader, fragmentShader) 
            topaz_material__set_program_source(REFi, language, vertexShader, fragmentShader);
        end

        local clearSources = function(REFi) 
            topaz_material__clear_sources(REFi);
        end

        return function(m) 
            m.__mapped = MAPPED;    
            m.getProgramData = getProgramData;
            m.setProgramData = setProgramData;
            m.setProgramSource = setProgramSource;
            m.clearSources = clearSources;
        end
    end)()

    initializer__image = (function() 

        local addFrame = function(REFi) 
            return topaz_image__add_frame(REFi);
        end
        local removeFrame = function(REFi, frame) 
            topaz_image__remove_frame(REFi, frame);
        end
        
        local getFrameCount = function(REFi) 
            return topaz_image__get_frame_count(REFi);
        end

        local getWidth = function(REFi) 
            return topaz_image__get_width(REFi);
        end
        local getHeight = function(REFi) 
            return topaz_image__get_height(REFi);
        end
        local resize = function(REFi, width, height) 
            topaz_image__resize(REFi, width, height);
        end
        
        local setFrameRGBAData = function(REFi, frame, rgbaData) 
            topaz_image__set_frame_rgba_data(REFi, frame, rgbaData);
        end
        
        local getFrameRGBAData = function(REFi, frame) 
            local output = {};
            local len;

            local getLen = function(a) 
                len = a;
            end
            
            local i = 0;
            local putColor = function(a, b, c, d) 
                output[i] = a; i = i + 1;
                output[i] = b; i = i + 1;
                output[i] = c; i = i + 1;
                output[i] = d; i = i + 1;

            end
            topaz_image__get_frame_rgba_data(REFi, frame, getLen, putColor);
            return output;
        end


        return function(i) 
            i.__mapped = MAPPED;
            i.addFrame = addFrame;
            i.removeFrame = removeFrame;
            i.getFrameCount = getFrameCount;
            i.getWidth = getWidth;
            i.getHeight = getHeight;
            i.resize = resize;
            i.setFrameRGBAData = setFrameRGBAData;
            i.getFrameRGBAData = getFrameRGBAData;
        end
    end)()

    local initializer__filesystem_path = (function() 

        local asString = function(REFi) 
            return topaz_filesystem_path__as_string(REFi);
        end
        
        local getName = function(REFi) 
            return topaz_filesystem_path__get_name(REFi);
        end
        
        local getParent = function(REFi) 
            local out = topaz_filesystem_path__get_parent(REFi);
            if (out.__mapped == nil) then
                initializer__filesystem_path(out);
            end
            return out;
        end
        
        local getChildren = function(REFi) 
            local output = {};
            for i=1, topaz_filesystem_path__get_child_count(REFi) do 
                output[i] = topaz_filesystem_path__get_nth_child(REFi, i-1);
                if (output[i].__mapped == nil) then
                    initializer__filesystem_path(output[i]);
                end
            end
            return output;
        end

        return function(f) 
            f.__mapped = MAPPED;
            f.asString = asString;
            f.getName = getName;
            f.getParent = getParent;
            f.getChildren = getChildren;
        end
    end)()

    local initializer__command_context = (function() 

        local destroy = function(REFi) 
            topaz_console__command_context_destroy(REFi);
        end
        
        local addCommand = function(REFi, command, callback) 
            local args = {};
            local argcount = 0;
            
            local addArg = function(a) 
                args[argcount] = a;  
                argcount=argcount+1;
            end

            local doAction = function(a) 
                local res = callback(
                    args,
                    a
                );
                args = {};
                argcount = 0;
                if (res == nil) then return '' end;
                return res;
            end   

            topaz_console__command_context_add_command(
                REFi,
                command,
                addArg,
                doAction
            )
        end
        
        local setDefaultHandler = function(REFi, handler) 
            local args = {};
            local argcount = 0;
            
            local addArg = function(a) 
                args[argcount] = a;  
                argcount=argcount+1;
            end

            local doAction = function(a) 
                local res = handler(
                    args,
                    a
                );
                args = {};
                argcount = 0;
                if (res == nil) then return '' end;
                return res;
            end       
            
            topaz_console__command_context_set_default_handler(
                REFi,
                addArg,
                doAction
            );        
        end
        
        local setPrompt = function(REFi, header) 
            topaz_console__command_context_set_prompt(REFi, header);
        end

        return function(c) 
            c.__mapped = MAPPED;
            c.destroy = destroy;
            c.addCommand = addCommand;
            c.setDefaultHandler = setDefaultHandler;
            c.setPrompt = setPrompt;
        end
    end)()


    local initializer__data = (function() 
        local setFromBytes = function(REFi, bytes) 
            topaz_data__set_from_bytes(REFi, bytes);
        end
        
        local setFromString = function(REFi, string) 
            topaz_data__set_from_string(REFi, string);
        end
        
        local setFromBase64 = function(REFi, base64) 
            topaz_data__set_from_base64(REFi, base64);
        end
        
        
        
        local getAsBytes = function(REFi) 
            local bytes = {};
            for i=1, topaz_data__get_byte_count(REFi) do 
                bytes[i] = topaz_data__get_nth_byte(REFi, i-1);
            end
            return bytes;
        end
        
        local getAsString = function(REFi) 
            return topaz_data__get_as_string(REFi);
        end
        
        local getAsBase64 = function(REFi) 
            return topaz_data__get_as_base64(REFi);
        end


        return function(d) 
            d.__mapped = MAPPED;
            d.setFromBytes = setFromBytes;
            d.setFromString = setFromString;
            d.setFromBase64 = setFromBase64;
            d.getAsBytes = getAsBytes;
            d.getAsString = getAsString;
            d.getAsBase64 = getAsBase64;
        end
    end)()




    local initializer__automation = (function() 

        
        local addKeyframe = function(REFi, value, func, offset) 
            topaz_automation__add_keyframe(REFi, value, func, offset);
        end
        
        local addVectorKeyframe = function(REFi, value, func, offset) 
            topaz_automation__add_vector_keyframe(REFi, value.x, value.y, value.z, func, offset);
        end
        
        local clear = function(REFi) 
            topaz_automation__clear(REFi);
        end
        
        local addAutomation = function(REFi, other) 
            topaz_automation__add_automation(REFi, other);
        end
        
        local blend = function(REFi, other) 
            topaz_automation__blend(REFi, other);
        end

        local smooth = function(REFi) 
            topaz_automation__smooth(REFi);
        end
        
        local addFromString = function(REFi, other) 
            topaz_automation__add_from_string(REFi, other);
        end
        
        local getLength = function(REFi) 
            return topaz_automation__get_length(REFi);
        end
        
        local skipTo = function(REFi, value) 
            topaz_automation__skip_to(REFi, value);
        end
        
        local setDurationSeconds = function(REFi, seconds) 
            topaz_automation__set_duration_seconds(REFi, seconds);
        end
        
        local setDurationFrames = function(REFi, frames) 
            topaz_automation__set_duration_frames(REFi, frames);
        end
        
        local getDuration = function(REFi) 
            return topaz_automation__get_duration(REFi);
        end

        local setLooped = function(REFi, loop) 
            topaz_automation__set_looped(REFi, loop);
        end

        local getLooped = function(REFi) 
            return topaz_automation__get_looped(REFi);
        end

        local setSpeed = function(REFi, speed) 
            topaz_automation__set_speed(REFi, speed);
        end

        local getSpeed = function(REFi) 
            return topaz_automation__get_speed(REFi);
        end
        
        local pause = function(REFi) 
            topaz_automation__pause(REFi);
        end

        local resume = function(REFi) 
            topaz_automation__resume(REFi);
        end
        
        local toString = function(REFi) 
            return topaz_automation__to_string(REFi);
        end
        
        local setFromString = function(REFi, automationString) 
            topaz_automation__set_from_string(REFi, automationString);
        end
        
        local temp = {};
        local tempSetter = function(a, b, c) 
            temp.x = a;
            temp.y = b;
            temp.c = c;
        end
        
        local vectorAt = function(REFi, progress) 
            topaz_automation__vector_at(REFi, progress, tempSetter);
            return copyVec(temp);
        end   
        
        local at = function(REFi, progress) 
            return topaz_automation__at(REFi, progress);
        end
        
        local currentVector = function(REFi) 
            topaz_automation__current_vector(REFi, tempSetter);
            return copyVec(temp);
        end
        
        local current = function(REFi) 
            return topaz_automation__current(REFi);
        end

        return function(a) 
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
        end
    end)()



    local initializer__display = (function() 

        local getViewport = function(REFi) 
            local out = topaz_display__get_viewport(REFi);
            if (out.__mapped == nil) then
                initializer__entity(out);
                initializer__viewport(out);
            end
            return out;
        end
        
        
        
        local setPostProcessShader = function(REFi, vertexShader, fragmentShader) 
            topaz_display__set_post_process_shader(REFi, vertexShader, fragmentShader);
        end

        
        
        local setParameter = function(REFi, param, value) 
            topaz_display__set_parameter(REFi, param, value);
        end
        
        local isParameterModifiable = function(REFi, param) 
            return topaz_display__is_parameter_modifiable(REFi, param);
        end
        
        local getParameter = function(REFi, param) 
            return topaz_display__get_parameter(REFi, param);
        end
       
        
        local setName = function(REFi, name) 
            topaz_display__set_name(REFi, name);
        end

        local addParameterCallback = function(REFi, callback) 
            return topaz_display__add_parameter_callback(REFi,function(a, b) callback(a, b) end);
        end

        local addCloseCallback = function(REFi, callback) 
            return topaz_display__add_close_callback(REFi, function(a) callback(a) end);
        end
        
        local removeCallback = function(REFi, id) 
            topaz_display__remove_callback(REFi, id);
        end
        
        
        local update = function(REFi) 
            topaz_display__update(REFi);
        end

        return function(d) 
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
        end

    end)()

    initializer__asset = (function() 

        local getType = function(REFi) 
            return topaz_asset__get_type(REFi);
        end

        local getName = function(REFi) 
            return topaz_asset__get_name(REFi);
        end

        
        return function(a) 
            a.__mapped = MAPPED;
            a.getType = getType;          
            a.getName = getName;          
        end
    end)()







    local initializer__component = (function() 

        local destroy = function(REFi) 
            topaz_component__destroy(REFi);
        end

        local detach = function(REFi) 
            topaz_component__detach(REFi);
        end

        local step = function(REFi) 
            topaz_component__step(REFi);
        end
        
        local draw = function(REFi) 
            topaz_component__draw(REFi);
        end
        
        local getStepping = function(REFi) 
            return topaz_component__get_stepping(REFi);
        end
        
        local getDrawing = function(REFi) 
            return topaz_component__get_drawing(REFi);
        end

        local setStepping = function(REFi, enable) 
            topaz_component__set_stepping(REFi, enable);
        end

        local setDrawing = function(REFi, enable) 
            topaz_component__set_drawing(REFi, enable);
        end
        
        local getTag = function(REFi) 
            return topaz_component__get_tag(REFi);
        end
        
        local getHost = function(REFi) 
            local e = topaz_component__get_host(REFi);
            if (e.__mapped == nil) then
                initializer__entity(e);
            end
            return e;

        end
        
        local emitEvent = function(REFi, event, source) 
            return topaz_component__emit_event(REFi, event, source);
        end
        
        local emitEventAnonymous = function(REFi, event) 
            return topaz_component__emit_event_anonymous(REFi, event);
        end
        
        local canHandleEvent = function(REFi, event) 
            return topaz_component__can_handle_event(REFi, event);
        end
        
        local installHook = function(REFi, event, hook) 
            return topaz_component__install_hook(REFi, event, function(a, b) hook(a, b) end);
        end
        
        local uninstallHook = function(REFi, event, id) 
            topaz_component__uninstall_hook(REFi, event, id);
        end

        local installHandler = function(REFi, event, handler) 
            return topaz_component__install_handler(REFi, event, function(a, b) handler(a, b) end);
        end
        
        local uninstallHandler = function(REFi, event, id) 
            topaz_component__uninstall_handler(REFi, event, id);
        end
        
        local installEvent = function(REFi, event, defaultHandler) 
            topaz_component__install_event(REFi, event, function(a, b) defaultHandler(a, b) end);
        end
        
        local uninstallEvent = function(REFi, event) 
            topaz_component__uninstall_event(REFi, event);
        end

        return function(c) 
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
        end
    end)()


    local initializer__shape2d = (function() 

        local tempColor = {};
        local tempColorSetter = function(a, b, c, d) 
            tempColor.r = a;
            tempColor.g = b;
            tempColor.b = c;
            tempColor.a = d;
        end   

        local getColor = function(REFi) 
            topaz_shape2d__get_color(REFi, tempColorSetter);
            return copyColor(tempColor);
        end
        
        local setColor = function(REFi, color) 
            topaz_shape2d__set_color(REFi, color.r, color.g, color.b, color.a);
        end
        
        local setPosition = function(REFi, value) 
            topaz_shape2d__set_position(REFi, value.x, value.y, value.z);
        end

        local setScale = function(REFi, value) 
            topaz_shape2d__set_scale(REFi, value.x, value.y, value.z);
        end

        local setRotation = function(REFi, value) 
            topaz_shape2d__set_rotation(REFi, value.x, value.y, value.z);
        end

        local temp = {};
        local tempSetter = function(a, b, c) 
            temp.x = a;
            temp.y = b;
            temp.z = c;
        end

        local getPosition = function(REFi) 
            topaz_shape2d__get_position(REFi, tempSetter);
            return copyVec(temp);
        end

        local getScale = function(REFi) 
            topaz_shape2d__get_scale(REFi, tempSetter);
            return copyVec(temp);
        end

        local getRotation = function(REFi) 
            topaz_shape2d__get_rotation(REFi, tempSetter);
            return copyVec(temp);
        end

        local getAnimSpeed = function(REFi) 
            return topaz_shape2d__get_anim_speed(REFi);
        end

        local setAnimSpeed = function(REFi, speed) 
            topaz_shape2d__set_anim_speed(REFi, speed);
        end
        
        local setCenter = function(REFi, value) 
            topaz_shape2d__set_center(REFi, value.x, value.y, value.z);
        end    

        local getCenter = function(REFi) 
            topaz_shape2d__get_center(REFi, tempSetter);
            return copyVec(temp);
        end
        
        local formRectangle = function(REFi, width, height) 
            topaz_shape2d__form_rectangle(REFi, width, height);
        end

        local formImage = function(REFi, asset) 
            topaz_shape2d__form_image(REFi, asset);
        end

        local formImageScaled = function(REFi, asset, width, height) 
            topaz_shape2d__form_image_scaled(REFi, asset, width, height);
        end
        
        local formImageFrame = function(REFi, asset, frame) 
            topaz_shape2d__form_image_frame(REFi, asset, frame);
        end

        local formImageFrameScaled = function(REFi, asset, frame, width, height) 
            topaz_shape2d__form_image_frame_scaled(REFi, asset, frame, width, height);
        end
        
        local formRadial = function(REFi, radius, numIterations) 
            topaz_shape2d__form_radial(REFi, radius, numIterations);
        end
        
        local formTriangles = function(REFi, vertices) 
            topaz_shape2d__form_triangles(REFi, vertices);
        end

        local formLines = function(REFi, vertices) 
            topaz_shape2d__form_lines(REFi, vertices);
        end

        local setAttribute = function(REFi, attribute, value) 
            topaz_shape2d__set_attribute(REFi, attribute, value);
        end

        local getAttribute = function(REFi, attribute) 
            return topaz_shape2d__get_attribute(REFi, attribute);
        end


        return function(c) 
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
        end
    end)()



    Topaz = { 
        Renderer = {
            TextureFilterHint = {
                Linear = 0,
                None = 1
            },
        
            Primitive = { 
                Triangle = 0,
                Line = 1
            },
            
            DepthTest = {
                Less = 0,
                LEQ = 1,
                Greater = 2,
                GEQ = 3,
                Equal = 4,
                None = 5
            },
            
            AlphaRule = {
                Allow = 0,
                Opaque = 1,
                Translucent = 2,
                Invisible = 3
            },
            
            EtchRule = {
                NoEtching = 0,
                Define = 1,
                Undefine = 2,
                In = 3,
                Out = 4
            },
        
            Attribute = {
                Primitive = 0,
                AlphaRule = 1,
                DepthTest = 2,
                EtchRle = 3,
                TextureFilterHint = 4
            },
            
            DataLayer = {
                Color = 1,
                Depth = 2,
                Etch  = 4,
                All   = 7
            }
        },

        Asset = {
            Type = { 
                None = 0,
                Image = 1,
                Sound = 2,
                Material = 3,
                Data = 4,
                Mesh = 5,
                Bundle = 6
            }
        },
        
        Entity = {
            create = (function() 

                return function(attributes) 
                    local entOut;
                    
                    if (attributes ~= nil) then
                        entOut = topaz_entity__create(
                            attributes.onAttach,
                            attributes.onDetach,
                            attributes.onRemove,
                            attributes.onPreStep,
                            attributes.onStep,
                            attributes.onPreDraw,
                            attributes.onDraw
                        )              
                    else 
                        entOut = topaz_entity__create()
                    end
                    
                    initializer__entity(entOut);
                    return entOut;
                end
            end)()
        },
        
        Component = {
            create = (function() 
                
                return function(tagName, attributes) 
                    local c = topaz_component__create(
                        tagName,
                        
                        attributes.onStep,
                        attributes.onDraw,
                        attributes.onAttach,
                        attributes.onDetach,
                        attributes.onDestroy
                    );
                    initializer__component(c);
                    return c;
                end        
            end)()
        },
        
        Automation = {
            Function = { 
                None = 0,
                Linear = 1,
                Accel = 2,
                Slow = 3,
                SoftAccel = 4,
                SoftSlow = 5,
                Random = 6
            },
            
            create = (function() 
                
                return function() 
                    local c = topaz_automation__create();
                    initializer__component(c);
                    initializer__automation(c);
                    return c;                
                end
            end)()
        },
        
        Shape2D = {
            create = (function()                 
                return function() 
                    local c = topaz_shape2d__create();
                    initializer__component(c);
                    initializer__shape2d(c);
                    return c;
                end
            end)()
        },
        
        
        Audio = (function() 
            return {
                playSound = function(asset, channel) 
                    return topaz_audio__play_sound(asset, channel);
                end,
                
                Channel = {
                    halt = function(channel) 
                        topaz_audio__channel_halt(channel);
                    end,
                    
                    setVolume = function(channel, amt) 
                        topaz_audio__channel_set_volume(channel, amt);
                    end,
                    
                    setPanning = function(channel, amt) 
                        topaz_audio__channel_set_panning(channel, amt);
                    end
                },
                
                Playback = {
                    setVolume = function(id, amt) 
                        topaz_audio__playback_set_volume(id, amt);
                    end,
                    setPanning = function(id, amt) 
                        topaz_audio__playback_set_panning(id, amt);
                    end,
                    setRepeat = function(id, rep) 
                        topaz_audio__playback_set_repeat(id, rep);                
                    end,
                    seek = function(id, to) 
                        topaz_audio__playback_seek(id, to);                                
                    end,
                    stop = function(id) 
                        topaz_audio__playback_stop(id);
                    end,
                    pause = function(id) 
                        topaz_audio__playback_pause(id);                
                    end,
                    resume = function(id) 
                        topaz_audio__playback_resume(id);                
                    end
                }
            }    
        end)(),
        
        ViewManager = (function() 
            
            return {
                createDisplay = function(displayName) 
                    local d = topaz_view_manager__create_display(displayName);
                    initializer__display(d);
                    return d;
                end,
                
                destroyDisplay = function(display) 
                    topaz_view_manager__destroy_display(display);
                end,
                
                getDefault = function() 
                    local d = topaz_view_manager__get_default();   
                    if (d.__mapped == nil) then
                        initializer__display(d);
                    end
                    return d;
                end,
                
                getClipboardAsString = function()
                    return topaz_view_manager__get_clipboard_as_string();
                end,
                
                setClipboardFromString = function(clipboardData) 
                    topaz_view_manager__set_clipboard_from_string(clipboardData);
                end,
                
                getAll = function()
                    local output = {};
                    for i=1, topaz_view_manager__get_display_count() do 
                        output[i] = topaz_view_manager__get_display(i-1);
                        if (output[i].__mapped == nil) then
                            initializer__display(output[i]);
                        end
                    end
                    return output;
                end,
            }
        end)(),
        
        Display = {
            ViewPolicy = {
                None      = 0,
                MatchSize = 1
            },

            Parameter = {
                X = 0,
                Y = 1,
                Width = 2,
                Height = 3,
                Show = 4,
                Fullscreen = 5,
                LockClientResize = 6,
                LockClientPosition = 7,
                ViewPolicy = 8,
                InputFocus = 9,
                Active = 10,
                Decorated = 11
            }
        },
        
        Viewport = (function()
            return {
                create = function()
                    local out = topaz_viewport__create();
                    initializer__entity(out);
                    initializer__viewport(out);
                    return out;
                end
            }
        end)(),

        Color = (function() 

            local colorTemp = {};
            local colorTemp_mod = function(a, b, c, d) 
                colorTemp.r = a;
                colorTemp.g = b;
                colorTemp.b = c;
                colorTemp.a = d;
            end

            return {
                toString = function(color) 
                    return topaz_color__to_hex_string(
                        color.r,
                        color.g,
                        color.b,
                        color.a
                    );
                end,
                
                fromString = function(str) 
                    topaz_color__set_from_string(str, colorTemp_mod);
                    return copyColor(colorTemp);
                end
            }
        end)(),
        
        Matrix = (function()
            local temp = {};
            local tempSetter = function(a, b, c) 
                temp.x = a;
                temp.y = b;
                temp.z = c;
            end
            
            local tempMatrix = {};
            local tempMatrixSetter = function(a, b, c, d, e, f, g, h, j, i , k , l, m ,n, o, p) 
                tempMatrix[1] = a;
                tempMatrix[2] = b;
                tempMatrix[3] = c;
                tempMatrix[4] = d;

                tempMatrix[5] = e;
                tempMatrix[6] = f;
                tempMatrix[7] = g;
                tempMatrix[8] = h;

                tempMatrix[9] = i;
                tempMatrix[10] = j;
                tempMatrix[11] = k;
                tempMatrix[12] = l;

                tempMatrix[13] = m;
                tempMatrix[14] = n;
                tempMatrix[15] = o;
                tempMatrix[16] = p;
            end
            
            return {
                setIdentity = function(m)
                    topaz_matrix__set_identity(m, tempMatrixSetter);
                    for i=1,16 do 
                        m[i] = tempMatrix[i]
                    end
                end,
                
                transform = function(m, point)
                    topaz_matrix__transform(m, point.x, point.y, point.z, tempSetter);
                    return copyVec(temp);
                end,
                
                toString = function(m) 
                    return topaz_matrix__to_string(m);
                end,
                
                transpose = function(m)
                    topaz_matrix__transpose(m, tempMatrixSetter);
                    for i=1,16 do 
                        m[i] = tempMatrix[i]
                    end                    
                end,

                reverseMajority = function(m) 
                    topaz_matrix__reverse_majority(m, tempMatrixSetter);
                    for i=1,16 do 
                        m[i] = tempMatrix[i]
                    end                                    
                end,
                
                multiply = function(a, b)
                    topaz_matrix__multiply(a, b, tempSetter);
                    return copyMatrix(tempMatrix);
                end,
                
                rotateByAngles = function(m, x, y, z)
                    topaz_matrix__rotate_by_angles(m, x, y, z, tempSetter);
                    for i=1,16 do 
                        m[i] = tempMatrix[i]
                    end                                    
                end,

                rotateByAxis = function(m, x, y, z, degrees)
                    topaz_matrix__rotate_by_axis(m, x, y, z, degrees, tempSetter);
                    for i=1,16 do 
                        m[i] = tempMatrix[i]
                    end                                    
                end,
                

                translate = function(m, x, y, z)
                    topaz_matrix__translate(m, x, y, z, tempSetter);
                    for i=1,16 do 
                        m[i] = tempMatrix[i]
                    end                                    
                end,
                
                scale = function(m, x, y, z)
                    topaz_matrix__scale(m, x, y, z, tempSetter);
                    for i=1,16 do 
                        m[i] = tempMatrix[i]
                    end                                    
                end,
                
                projectionPerspective = function(fovy, ratio, zNear, zFar)
                    topaz_matrix__projection_perspective(fovy, ratio, zNear, zFar, tempSetter);
                    return copyMatrix(tempMatrix);
                end,                
                
                projectionOrthographic = function(left, right, bottom, top, zNear, zFar) 
                    topaz_matrix__projection_orthographic(left, right, bottom, top, zNear, zFar, tempSetter);
                    return copyMatrix(tempMatrix);
                end
            }            
        end)(),
        
        Vector = (function() 
            
            local temp = {};
            local tempSetter = function(a, b, c) 
                temp.x = a;
                temp.y = b;
                temp.z = c;
            end
            
            
            return {
                fromString = function(input) 
                    topaz_vector__from_string(input, tempSetter);
                    return copyVec(temp);
                end,
                reset = function(v) 
                    topaz_vector__reset(tempSetter);
                    v.x = temp.x;
                    v.y = temp.y;
                    v.z = temp.z;
                end,
                getLength = function(v) 
                    return topaz_vector__get_length(v.x, v.y, v.z);
                end,
                
                getDistance = function(p0, p1) 
                    return topaz_vector__get_distance(
                        p0.x, p0.y, p0.z,
                        p1.x, p1.y, p1.z
                    )
                end,
                
                normalize = function(v) 
                    topaz_vector__normalize(v.x, v.y, v.z, tempSetter);
                    v.x = temp.x;
                    v.y = temp.y;
                    v.z = temp.z;
                end,
                
                cross = function(a, b) 
                    topaz_vector__cross(
                        a.x, a.y, a.z,
                        b.x, b.y, b.z,
                        tempSetter
                    );
                    return copyVec(temp);                
                end,
                
                dot = function(a, b) 
                    topaz_vector__dot(
                        a.x, a.y, a.z,
                        b.x, b.y, b.z,
                        tempSetter
                    );
                    return copyVec(temp);                
                end,
                floor = function(v) 
                    topaz_vector__floor(v.x, v.y, v.z, tempSetter);
                    v.x = temp.x;
                    v.y = temp.y;
                    v.z = temp.z;
                end,  
                
                reflect2d = function(direction, surface) 
                    topaz_vector__reflect_2d(
                        direction.x, direction.y, direction.z,
                        surface.x, surface.y, surface.z,
                        tempSetter
                    )
                    return copyVec(temp);
                end,
                
                lookAtRotation = function(p0, p1, up) 
                    topaz_vector__look_at_rotation(
                        p0.x, p0.y, p0.z,
                        p1.x, p1.y, p1.z,
                        up.x, up.y, up.z,
                        tempSetter
                    );
                    return copyVec(temp);
                end,
                
                
                pointAt2D = function(p0, p1) 
                    return topaz_vector__point_at_2d(
                        p0.x, p0.y, p0.z,
                        p1.x, p1.y, p1.z
                    )
                end
            }
        end)(),
        
        Console = (function()             
            return {
                MessageType = {
                    Normal = 0,
                    Debug = 1,
                    Warning = 2,
                    Error = 3
                },
                
                enable = function(enable) 
                    topaz_console__enable(enable);
                end,
                
                ['print'] = function(message) 
                    topaz_console__print(message);
                end,
                
                printMessage = function(message, type) 
                    topaz_console__print_message(message, type);
                end,
                
                addListener = function(callback) 
                    return topaz_console__add_listener(function(a, b) callback(a, b) end);
                end,
                
                removeListener = function(id) 
                    topaz_console__remove_listener(id);
                end,
                
                run = function(command) 
                    topaz_console__run(command);
                end,
                
                CommandContext = {
                    create = function()
                        local out = topaz_console__command_context_create();
                        initializer__command_context(out);
                        return out;
                    end
                },
                
                pushCommandContext = function(context) 
                    topaz_console__push_command_context(context);
                end,
                
                popCommandContext = function()
                    topaz_console__pop_command_context();
                end
            }
        end)(),
        
        Filesystem = (function() 
            
            return {
                DefaultNode = {
                    Resources = 0,
                    Topaz = 1,
                    UserData = 2,
                    Bundles = 3
                },
                
                
                getPath = function(node) 
                    local out = topaz_filesystem__get_path(node);
                    if (out.__mapped == nil) then
                        initializer__filesystem_path(out);
                    end
                    return out;
                end,
                
                getPathFromString = function(from, path) 
                    local out = topaz_filesystem__get_path_from_string(from, path);
                    if (out.__mapped == nil) then
                        initializer__filesystem_path(out);
                    end
                    return out;
                end
            }
        end)(),
        
        FontManager = (function() 
            
            return {
                registerFont = function(asset) 
                    return topaz_font_manager__register_font(asset);
                end,
                
                preloadGlyphs = function(font, sizeRequest, characters) 
                    topaz_font_manager__preload_glyphs(font, sizeRequest, characters);
                end,
                
                unregisterFont = function(font) 
                    topaz_font_manager__unregister_font(font);
                end
                
            }
        end)(),
        
        Input = (function() 
            local getListener = function(listener) 
                local onPress   = listener.onPress;
                local onActive  = listener.onActive;
                local onRelease = listener.onRelease;
                local onUpdate  = listener.onUpdate;
        
                local output = {};
                
                if(onPress)   then output.onPress = function(a)    onPress(a) end end
                if(onActive)  then output.onActive = function(a, b) onActive(a, b) end end
                if(onRelease) then output.onRelease = function(a)    onRelease(a) end end
                if(onUpdate)  then output.onUpdate = function(a, b) onUpdate(a, b) end end

        
                return output;
            end

            return {
                addKeyboardListener = function(listener) 
                    return topaz_input__add_keyboard_listener(
                        getListener(listener)
                    );
                end,

                addPointerListener = function(listener) 
                    return topaz_input__add_pointer_listener(
                        getListener(listener)
                    );
                end,

                addPadListener = function(listener, padIndex) 
                    return topaz_input__add_pad_listener(
                        getListener(listener),
                        padIndex
                    );
                end,
                
                addMappedListener = function(listener, mappedName) 
                    return topaz_input__add_mapped_listener(
                        getListener(listener),
                        mappedName
                    );            
                end,
                
                removeListener = function(id) 
                    topaz_input__remove_listener(id);
                end,
                
                getState = function(input) 
                    return topaz_input__get_state(input);
                end,
                
                getMappedState = function(input, name) 
                    return topaz_input__get_mapped_state(input, name);
                end,
                
                setDeadzone = function(padID, input, deadZone) 
                    return topaz_input__set_deadzone(padID, input, deadZone);
                end,
                
                queryPads = function()
                    local output = {};
                    for i=1, topaz_input__query_pad_count() do
                        output[i] = topaz_input__query_pad_id(i-1);
                    end
                    return output;
                end,
                
                addUnicodeListener = function(listener) 
                    local onNewUnicode = listener.onNewUnicode;
                    local onRepeatUnicode = listener.onRepeatUnicode;
                    
                    local output = {};
                    
                    if (onNewUnicode) then output.onNewUnicode = function(a) onNewUnicode(a) end end
                    if (onRepeatUnicode) then output.onRepeatUnicode = function(a) onRepeatUnicode(a) end end
                    
                    
                    return topaz_input__add_unicode_listener(
                        output
                    );
                end,
                
                removeUnicodeListener = function(id) 
                    topaz_input__remove_unicode_listener(id);
                end,
                
                map = function(name, input) 
                    topaz_input__map(name, input);
                end,
                
                mapPad = function(name, pad, input) 
                    topaz_input__map_pad(name, pad, input);
                end,
                unmap = function(name) 
                    topaz_input__unmap(name);
                end,
                
                mouseX = function()
                    return topaz_input__mouse_x();
                end,
                
                mouseY = function()
                    return topaz_input__mouse_y();
                end,

                mouseDeltaX = function ()
                    return topaz_input__mouse_delta_x();
                end,    

                mouseDeltaY = function()
                    return topaz_input__mouse_delta_y();
                end,
                
                mouseWheel = function()
                    return topaz_input__mouse_wheel();
                end
            }    
        end)(),

        Key = {
            _0= 1, --/< 0
            _1= 2, --/< 1
            _2= 3, --/< 2
            _3= 4, --/< 3
            _4= 5, --/< 4
            _5= 6, --/< 5
            _6= 7, --/< 6
            _7= 8, --/< 7
            _8= 9, --/< 8
            _9= 10, --/< 9
            a= 11, --/< a
            b= 12, --/< b
            c= 13, --/< c
            d= 14, --/< d
            e= 15, --/< e
            f= 16, --/< f
            g= 17, --/< g
            h= 18, --/< h
            i= 19, --/< i
            j= 20, --/< j
            k= 21, --/< k
            l= 22, --/< l
            m= 23, --/< m
            n= 24, --/< n
            o= 25, --/< o
            p= 26, --/< p
            q= 27, --/< q
            r= 28, --/< r
            s= 29, --/< s
            t= 30, --/< t
            u= 31, --/< u
            v= 32, --/< v
            w= 33, --/< w
            x= 34, --/< x
            y= 35, --/< y
            z= 36, --/< z
            lshift= 37, --/< Left shift key
            rshift= 38, --/< Right shift key
            lctrl= 39,  --/< Left control key
            rctrl= 40,  --/< Right control key
            lalt= 41,   --/< Left alt key
            ralt= 42,   --/< Right alt key
            tab= 43,    --/< Tab
            F1= 44,     --/< F1
            F2= 45,     --/< F2
            F3= 46,     --/< F3
            F4= 47,     --/< F4
            F5= 48,     --/< F5
            F6= 49,     --/< F6
            F7= 50,     --/< F7
            F8= 51,     --/< F8
            F9= 52,     --/< F9
            F10= 53,    --/< F10
            F11= 54,    --/< F11
            F12= 55,    --/< F12
            up = 100,     --/< Up arrow
            down= 101,   --/< Down arrow
            left= 102,   --/< Left arrow
            right= 103,  --/< Right arrow
            minus= 104,  --/< -
            equal = 105,  --/< = 
            backspace = 106,  --/< Backspace
            grave= 107,  --/< `
            esc= 108,    --/< Escape
            home= 109,   --/< Home key
            pageUp= 110, --/< Page up key
            pageDown = 111,  --/< Page down key
            ['end'] = 112,    --/< End key
            backslash= 113, --/< '\'
            lbracket= 114, --/< [
            rbracket= 115, --/< ]
            semicolon= 116, --/< ;
            apostrophe= 117, --/< '
            frontslash= 118, --/< /
            enter= 119, --/< Enter
            delete= 120, --/< Delete
            numpad0= 121, --/< Numpad 0
            numpad1= 122, --/< Numpad 1
            numpad2= 123, --/< Numpad 2
            numpad3= 124, --/< Numpad 3
            numpad4= 125, --/< Numpad 4
            numpad5= 126, --/< Numpad 5
            numpad6= 127, --/< Numpad 6
            numpad7= 128, --/< Numpad 7
            numpad8= 129, --/< Numpad 8
            numpad9= 130, --/< Numpad 9
            prtscr= 131, --/< Print screen button
            lsuper= 132, --/< Left Super key (Windows key)
            rsuper= 133, --/< Right Super key (Windows key)
            space= 134,  --/< Space
            insert= 135, --/< Insert key
            comma= 136, --/< ,
            period= 137 , --/< .
            world1= 138, --/ I8n key0
            world2= 139, --/ I8n key1
            world3= 140, --/ I8n key2
            world4= 141, --/ I8n key3
            world5= 142, --/ I8n key4
            world6= 143, --/ I8n key5
            world7= 144, --/ I8n key6
            world8= 145, --/ I8n key7
            world9= 146 --/ I8n key8
        },



        Pointer = {
            _0= 256, --/< Left click
            _1= 257, --/< Right click
            _2= 258, --/< Middle click
        
            X= 259, --/< Horizontal axis. Usually for the X axis of the pointer
            Y= 260, --/< Horizontal axis. Usually for the X axis of the pointer
            wheel= 261 --/< Mouse wheel.
        },

        Pad = {
        
            a= 300,     --/< Button 0
            b= 301,     --/< Button 1
            c= 302,     --/< Button 2
            x= 303,     --/< Button 3
            y= 304,     --/< Button 4
            r= 305,     --/< Button 5
            l= 306,     --/< Button 6
            r2= 307,    --/< Button 7
            l2= 308,    --/< Button 8
            r3= 309,    --/< Button 9
            l3= 310,    --/< Button 10
            start= 311,    --/< Button 11
            ['select']= 312,--/< Button 12
            b13= 313,--/< Button 13
            d_up= 314,--/< Button DPAD UP
            d_right= 315,--/< Button DPAD RIGHT
            d_down= 316,--/< Button DPAD DOWN
            d_left= 317,--/< Button DPAD LEFT
            b18= 318,--/< Button 18
            b19= 319,--/< Button 19
            b20= 320,--/< Button 20
            b21= 321,--/< Button 21
            b22= 322,--/< Button 22
            b23= 323,--/< Button 23
            b24= 324,--/< Button 24
            b25= 325,--/< Button 25
            b26= 326,--/< Button 26
            b27= 327,--/< Button 27
            b28= 328,--/< Button 28
            b29= 329,--/< Button 29
            b30= 330,--/< Button 30
            b31= 331,--/< Button 31
            b32= 332,--/< Button 32
        
            axisX= 400, --/< X button
            axisY= 401, --/< Y button
            axisZ= 402, --/< Z button
            axisX2= 403,--/< X2 button 
            axisY2= 404,--/< Y2 button
            axisZ2= 405,--/< Z2 button
            axisX3= 406,--/< X3 button
            axisY3= 407,--/< Y3 button
            axisZ3= 408,--/< Z3 button
            axisX4= 409,--/< X4 button
            axisY4= 410,--/< Y4 button
            axisZ4= 411,--/< Z4 button
            axisX5= 412,--/< X4 button
            axisY5= 413,--/< Y4 button
            axisZ5= 414,--/< Z4 button
            axisX6= 415,--/< X4 button
            axisY6= 416,--/< Y4 button
            axisZ6= 417,--/< Z4 button
        
            axisR= 450,   
            axisL= 451,    
            axisR2= 452,    
            axisL2= 453,    
            axisR3= 454,    
            axisL3= 455,    
            axisR4= 456,    
            axisL4= 457,    
        
            options= 511
        },
        
        Object2D = (function() 
        
            return {
                Group = {
                    A = 0,
                    B = 1,
                    C = 2,
                    D = 3,
                    E = 4,
                    F = 5,
                    G = 6,
                    H = 7,
                    I = 8,
                    J = 9,
                    K = 10,
                    L = 11,
                    M = 12,
                    N = 13,
                    O = 14,
                    P = 15,
                    Q = 16,
                    R = 17,
                    S = 18,
                    T = 19,
                    U = 20,
                    V = 21,
                    W = 22,
                    X = 23,
                    Y = 24,
                    Z = 25
                },
                
                create = function()
                    local out = topaz_object2d__create();
                    initializer__object2d(out);
                    initializer__component(out);
                    return out;
                end,
                
                setGroupInteraction = function(a, b, shouldInteract) 
                    topaz_object2d__set_group_interaction(a, b, shouldInteract);
                end
            }
        end)(),
        
        Particle = (function() 
            
            return {
                Property = {
                    Duration = 0,
                    ScaleX = 1,
                    ScaleY = 2,
                    ScaleMultiplier = 3,
                    Rotation = 4,
                    Direction = 5,
                    SpeedX = 6,
                    SpeedY = 7,
                    Red = 8,
                    Green = 9,
                    Blue = 10,
                    Alpha = 11
                } ,
                
                create = function()
                    local p = topaz_particle__create();
                    initializer__particle(p);
                    return p;
                end
            }
        end)(),
        
        ParticleEmitter2D = (function() 
            return {
                create = function()
                    local out = topaz_particle_emitter_2d__create();
                    initializer__particle_emitter_2d(out);
                    initializer__entity(out);
                    return out;
                end
            }   
        end)(),
        
        Resources = (function() 

            return {
                setPath = function(path) 
                    return topaz_resources__set_path(path);
                end,
                
                getPath = function()
                    return topaz_resources__get_path();
                end,
                
                createAsset = function(name, type) 
                    local out = topaz_resources__create_asset(name, type);
                    if (out == nil) then return nil end;
                    initializer__asset(out);
                    local typeO = Topaz.Asset.Type;
                    if     (type == typeO.Image) then initializer__image(out)
                    elseif (type == typeO.Sound) then initializer__sound(out)
                    elseif (type == typeO.Material) then initializer__material(out)
                    elseif (type == typeO.Data) then initializer__data(out)
                    elseif (type == typeO.Mesh) then initializer__mesh(out) 
                    end
                    
                    return out;
                end,
                
                fetchAsset = function(name) 
                    local out = topaz_resources__fetch_asset(name);
                    if (out == nil) then return nil end;
                    if (out.__mapped) then return out end;
                    initializer__asset(out);
                    local typeO = Topaz.Asset.Type;
                    local type = out:getType();
                    if     (type == typeO.Image) then initializer__image(out)
                    elseif (type == typeO.Sound) then initializer__sound(out)
                    elseif (type == typeO.Material) then initializer__material(out)
                    elseif (type == typeO.Data) then initializer__data(out)
                    elseif (type == typeO.Mesh) then initializer__mesh(out) 
                    end


                    
                    return out;                
                end,
                
                createDataAssetFromPath = function(path, name) 
                    local out = topaz_resources__create_data_asset_from_path(path, name);
                    if (out == nil) then return nil end;
                    initializer__asset(out);
                    initializer__data(out);
                    return out;
                end,
                
                convertAsset = function(fileType, asset) 
                    local out = topaz_resources__convert_asset(fileType, asset);
                    if (out == nil) then return nil end;
                    initializer__asset(out);
                    local typeO = Topaz.Asset.Type;
                    local type = out:getType();
                    if     (type == typeO.Image) then initializer__image(out)
                    elseif (type == typeO.Sound) then initializer__sound(out)
                    elseif (type == typeO.Material) then initializer__material(out)
                    elseif (type == typeO.Data) then initializer__data(out)
                    elseif (type == typeO.Mesh) then initializer__mesh(out) 
                    end


                    
                    
                    return out;                 
                end,
                
                writeAsset = function(asset, fileType, outputPath) 
                    return topaz_resources__write_asset(asset, fileType, outputPath);
                end,
                
                removeAsset = function(asset) 
                    topaz_resources__remove_asset(asset);
                end,
                
                isExtensionSupported = function(name) 
                    return topaz_resources__is_extension_supported(name);
                end,
                
                packBundle = function(assetName, header, depends, assets) 
                    local out = topaz_resources__pack_bundle(assetName, header, depends, assets);
                    if (out == nil) then return nil end;
                    initializer__asset(out);
                    initializer__data(out);
                    return out;
                end,
                
                unpackBundle = function(bundleName, min_majorVersionRequired, min_minorVersionRequired, onNewItem) 
                    return topaz_resources__unpack_bundle(bundleName,
                        min_majorVersionRequired,
                        min_minorVersionRequired,
                        onNewItem 
                    );
                end,
                
                queryBundle = function(bundleName) 
                    return topaz_resources__query_bundle(bundleName);
                end
            }
        end)(),
        
        RNG = (function() 

            return {
                create = function()
                    local out = topaz_rng__create();
                    initializer__rng(out);
                    return out
                end
            }
        end)(),
        
        Scheduler = (function() 
            return {
                Mode = {
                    Time = 0,
                    Frame = 1
                },
                create = function(
                    mode,
                    justOnce,
                    interval,
                    intervalDelay,
                    callback
                ) 
                    local out = topaz_scheduler__create(mode, justOnce, interval, intervalDelay, callback);
                    initializer__scheduler(out);
                    initializer__component(out);
                    return out;
                end
            }        
        end)(),
        
        Shape3D = (function() 
            return { 
                create = function()
                    local out = topaz_shape3d__create();
                    initializer__component(out);
                    initializer__shape3d(out);
                    return out;
                end
            }
        end)(),
        
        StateControl = (function() 
            return {
                create = function()
                    local out = topaz_state_control__create();
                    initializer__component(out);
                    initializer__state_control(out);
                    return out;                
                end
            }   
        end)(),
        
        Text2D = (function() 
            return {
                create = function()
                    local out = topaz_text2d__create();
                    initializer__component(out);
                    initializer__text2d(out);
                    return out;
                end
            }
        end)(),
        
        run   = topaz__run,
        pause   = topaz__pause,
        pauseNow = topaz__pause_now,
        resume = topaz__resume,
        iterate = topaz__iterate,
        isPaused = topaz__is_paused,
        debug = (function() 
            return function()
                topaz__debug();
            end
        end)(),
        quit = topaz__quit,
        getTime = topaz__get_time,
        getDeltaTime = topaz__get_delta_time,

        getVersionMinor = topaz__get_version_minor,
        getVersionMicro = topaz__get_version_micro,
        getVersionMajor= topaz__get_version_major,
        
        frameEnd = topaz__frame_end

    }


    (function() 

        Topaz.attachPreManager = function(entity) 
            topaz__attach_pre_manager(entity);
        end
        
        Topaz.attachPreManagerUnpausable = function(entity) 
            topaz__attach_pre_manager_unpausable(entity);
        end

        Topaz.attachPostManager = function(entity) 
            topaz__attach_post_manager(entity);
        end
        
        Topaz.attachPostManagerUnpausable = function(entity) 
            topaz__attach_post_manager_unpausable(entity);
        end


        Topaz.frameStart = function(FPS) 
            topaz__frame_start(FPS);
        end
        
        Topaz.Lua = {
        
            import = function(path)
                local asset = Topaz.Resources.fetchAsset(path);
                if (asset == nil) then 
                    asset = Topaz.Resources.createDataAssetFromPath(path, path);
                else 
                    return; --Already loaded.
                end
                
                if (asset == nil) then 
                    error('Could not load lua script from path '..path..'!');
                    return
                end

                if (asset:getType() ~= Topaz.Asset.Type.Data) then 
                    error('Tried to load lua script '..path..', but it isn\'t a data asset!');
                end

                topaz_script__run(path, asset:getAsString());
            end,
            
            getLocal = function(l, level)
                local val;
                local funcIter = level;
                if (funcIter == nil) then 
                    funcIter = 2;
                end
                
                if (funcIter < 2) then 
                    error('Cannot be level 1 or lower');
                end
                
                local fInfo = debug.getinfo(funcIter); 
                if (fInfo == nil) then
                    error("Could not access invalid function index.");
                end

                local localIter = 1;
                while(true) do 
                    name, value = debug.getlocal(funcIter, localIter);
                    if (name == nil) then
                        break;
                    end
                    
                    Topaz.Console.print('name: '..tostring(name)..', value: '..tostring(value));

                    if (name == l) then 
                        return value
                    end
                        

                    localIter = localIter + 1;
                end
                
                localIter = 1
                while(true) do 
                    name, value = debug.getupvalue(fInfo.func, localIter);
                    if (name == nil) then
                        break;
                    end
                    
                    Topaz.Console.print('name: '..tostring(name)..', value: '..tostring(value));

                    if (name == l) then 
                        return value
                    end
                        

                    localIter = localIter + 1;
                end                    
                
                -- Fallback on global index
                if (val == nil) then
                    return _G[l];
                else
                    return val;
                end
            end,
            
            
            objectToString = function(value)
                local already = {}
                local pspace = function(level) 
                    local str = '';
                    for i=1,level do
                        str = str..'  ';
                    end
                    return str;
                end
                
                
                local helper
                
                helper = function(obj, level)
                    local poself = helper;

                    local t= type(obj);
                    if    (t == 'string')   then return '(string): \''..obj..'\''
                    elseif(t == 'number')   then return '(number): '..obj 
                    elseif(t == 'boolean')  then return '(boolean): '..obj
                    elseif(t == 'function') then return '(function) '
                    elseif(t == 'nil')      then return '<nil>'
                    elseif(t == 'table')    then
                        if (already[obj] == true) then
                            return '(table): [already printed]';
                        end

                        already[obj] = true;

                        local output = '(table): {';

                        local multi = false;
                        for key,val in pairs(obj) do                        
                            if (multi) then 
                                output = output..',\n'; 
                            else 
                                output = output..'\n'; 
                            end

                            output = output..pspace(level+1)..(''..key)..' = '..poself(val, level+1);
                            multi = true;
                        end
                        
                        if (multi) then 
                            output = output..pspace(level)..'\n'..pspace(level)..'}'
                        else 
                            output = output..pspace(level)..'}'    
                        end
                        
                        return output;                
                    else 
                        return ''
                    end
                end
                return pspace(1) .. helper(value, 1);
            end
        }
    end)();

end)()
