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
subdir(handlerPreferredFilesys)
subdir(handlerPreferredInputManager)
subdir(handlerPreferredRenderer)
subdir(handlerPreferredTime)

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
        depCompiled = dependency(dep.get('name'), required:false)

        if depCompiled.found() == false
            depCompiled = dependency(dep.get('alternateName'), required:false)
            if depCompiled.found() == false
                message(''+local__name+':'+local__type+' supported? NO (missing dependency: ' + dep + ')')
                error('Requested backend cannot be used.')
                break
            endif
        endif
        message('Requested dependency: ' + depCompiled.type_name() + ' : ' + depCompiled.version())
        local__dependenciesCompiled += depCompiled
    endforeach


    if local__shouldTest == true 
        resultObject = cc.run(files('../system/'+local__type+'_'+local__name+'/compat_test.c'), dependencies : local__dependenciesCompiled)

        if resultObject.compiled() == true
            handlerInternalSource   += 'topaz_system_config_add_handler(TOPAZ_STR_CAST("'+local__type+'"), TOPAZ_STR_CAST("'+local__name+'"), topaz_system_'+local__type+'_'+local__name+'__backend, (void (*)(void *)) topaz_system_'+local__type+'_'+local__name+'__api);\n'
            handlerInternalInclude  += '#include "../system/'+local__type+'_'+local__name+'/src/backend.h"\n'
            handlerSources += local__sources
            handlerDependencies += local__dependenciesCompiled
            message(''+local__name+':'+local__type+' supported? YES')
        else
            message(''+local__name+':'+local__type+' supported? NO')
            error('Requested backend cannot be used.')
        endif

    else 
        handlerInternalSource   += 'topaz_system_config_add_handler(TOPAZ_STR_CAST("'+local__type+'"), TOPAZ_STR_CAST("'+local__name+'"), topaz_system_'+local__type+'_'+local__name+'__backend, (void (*)(void *)) topaz_system_'+local__type+'_'+local__name+'__api);\n'
        handlerInternalInclude  += '#include "../system/'+local__type+'_'+local__name+'/src/backend.h"\n'
        handlerSources += local__sources
        handlerDependencies += local__dependenciesCompiled
        
        message(''+local__name+':'+local__type+' supported? YES (auto-pass)')

    endif

endforeach
