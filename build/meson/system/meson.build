# basic placeholder backends. These exist so that 
# if nothing is supported, the program can run partially.

# each element is 
# {
#    name : 'name', 
#    type : 'type'
#    sources : ['source1.c', 'source2.c'],
#    shouldTest : true/false
# }
#
#
#
#
handlers = []



subdir(handlerPreferredAudioManager)
subdir(handlerPreferredDisplay)
subdir(handlerPreferredFilesystem)
subdir(handlerPreferredInputManager)
subdir(handlerPreferredRenderer)
subdir(handlerPreferredTime)
subdir(handlerPreferredScript)
subdir(handlerPreferredConsoleDisplay)
subdir(handlerPreferredFontRenderer)
foreach iox : handlerPreferredIOXs
    subdir(iox)
endforeach


# actually process results
foreach handler : handlers 
    local__type = handler.get('type', 'ERROR')
    local__name = handler.get('name', 'ERROR')
    local__shouldTest = handler.get('shouldTest', false)
    local__sources = handler.get('sources', [])
    local__dependencies = handler.get('dependencies', [])

    # has actual dependency objects
    local__dependenciesCompiled = []

    # make sure all dependencies are valid
    foreach dep : local__dependencies
        if dep.has_key('static') == true 
            local__dep_static = true 
        else 
            local__dep_static = false
        endif

        depCompiled = dependency(dep.get('name'), required:false, static:local__dep_static)

        if depCompiled.found() == false
            depCompiled = dependency(dep.get('alternateName'), required:false, static:local__dep_static)
            if depCompiled.found() == false
                depCompiled = meson.get_compiler('c').find_library(dep.get('name'))
                if depCompiled.found() == false
                    message(''+local__name+':'+local__type+' supported? NO (missing dependency: ' + dep.get('name') + ')')
                    error('Requested backend cannot be used.')
                    break
                endif
            endif
        endif
        message('Requested dependency: ' + depCompiled.type_name())
        local__dependenciesCompiled += depCompiled
    endforeach


    if local__shouldTest == true 
        resultObject = cc.run(files('../../../system/'+local__type+'/'+local__name+'/compat_test.c'), dependencies : local__dependenciesCompiled)

        if resultObject.compiled() == true
            handlerInternalSource   += 'topaz_system_config_add_handler(TOPAZ_STR_CAST("'+local__type+'"), TOPAZ_STR_CAST("'+local__name+'"), (void (*)(topazSystem_t *, topazSystem_Backend_t *, void * api))topaz_system_'+local__type+'_'+local__name+'__backend);\n'
            handlerInternalInclude  += '#include "../../../system/'+local__type+'/'+local__name+'/src/backend.h"\n'
            handlerSources += local__sources
            handlerDependencies += local__dependenciesCompiled
            message(''+local__name+':'+local__type+' supported? YES')
        else
            message(''+local__name+':'+local__type+' supported? NO')
            error('Requested backend cannot be used.')
        endif

    else 
        handlerInternalSource   += 'topaz_system_config_add_handler(TOPAZ_STR_CAST("'+local__type+'"), TOPAZ_STR_CAST("'+local__name+'"), (void (*)(topazSystem_t *, topazSystem_Backend_t *, void * api))topaz_system_'+local__type+'_'+local__name+'__backend);\n'
        handlerInternalInclude  += '#include "../../../system/'+local__type+'/'+local__name+'/src/backend.h"\n'
        handlerSources += local__sources
        handlerDependencies += local__dependenciesCompiled
        
        message(''+local__name+':'+local__type+' supported? YES (auto-pass)')

    endif

endforeach
