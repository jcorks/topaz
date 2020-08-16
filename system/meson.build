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

subdir('audioManager_noAudioManager')
subdir('display_noDisplay')
subdir('filesys_noFilesys')
subdir('inputManager_noInputManager')
subdir('renderer_noRenderer')
subdir('time_noTime')


# base backends recommended by the project 
subdir('filesys_posix')
subdir('renderer_srgs')
subdir('time_posix')



# actually process results
foreach handler : handlers 
    local__type = handler.get('type', 'ERROR')
    local__name = handler.get('name', 'ERROR')
    local__shouldTest = handler.get('shouldTest', false)
    local__sources = handler.get('sources', [])

    if local__shouldTest == true 
        resultObject = cc.run('system/'+local__type+'_'+'local__name+/compat_test.c')

        if resultObject.compiled() == true
            handlerInternalSource   += 'topaz_system_config_add_handler(TOPAZ_STR_CAST("'+local__type+'"), TOPAZ_STR_CAST("'+local__name+'"), topaz_system_'+local__type+'_'+local__name+'__backend, (void (*)(void *)) topaz_system_'+local__type+'_'+local__name+'__api);\n'
            handlerInternalInclude  += '#include "../system/'+local__type+'_'+local__name+'/src/backend.h"\n'
            handlerSources += local__sources
            message(''+local__name+':'+local__type+' supported? YES')
        else
            message(''+local__name+':'+local__type+' supported? NO')
        endif

    else 
        handlerInternalSource   += 'topaz_system_config_add_handler(TOPAZ_STR_CAST("'+local__type+'"), TOPAZ_STR_CAST("'+local__name+'"), topaz_system_'+local__type+'_'+local__name+'__backend, (void (*)(void *)) topaz_system_'+local__type+'_'+local__name+'__api);\n'
        handlerInternalInclude  += '#include "../system/'+local__type+'_'+local__name+'/src/backend.h"\n'
        handlerSources += local__sources
        
        message(''+local__name+':'+local__type+' supported? YES (auto-pass)')

    endif

endforeach