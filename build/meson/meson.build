project(
    'Topaz', 
    'c',
    version : '0.1.0'
)
message('Host OS: ' + host_machine.system())

# This is the root of all includes.
# includes are namespaced properly

mainIncdir = include_directories('../../include')


# base source files. 
# Add these as needed
src = [
    'src/asset.c',
    'src/assets_image.c',
    'src/assets_data.c',
    'src/backends_audio_manager.c',
    'src/backends_console_display.c',
    'src/backends_display.c',
    'src/backends_filesys.c',
    'src/backends_font_renderer.c',
    'src/backends_input_manager.c',
    'src/backends_renderer.c',
    'src/backends_time.c',
    'src/backends_script.c',
    'src/backends_iox.c',
    'src/camera.c',
    'src/color.c',
    'src/component.c',
    'src/components_scheduler.c',
    'src/components_shape2d.c',
    'src/components_state_control.c',
    'src/components_object2d.c',
    'src/components_text2d.c',
    'src/components_automation.c',
    'src/containers_array.c',
    'src/containers_bin.c',
    'src/containers_string.c',
    'src/containers_table.c',
    'src/entity.c',
    'src/input_device.c',
    'src/material.c',
    'src/math.c',
    'src/matrix.c',
    'src/mesh.c',
    'src/modules_input.c',
    'src/modules_resources.c',
    'src/modules_view_manager.c',
    'src/modules_graphics.c',
    'src/modules_script_manager.c',
    'src/modules_console.c',
    'src/rbuffer.c',
    'src/refbank.c',
    'src/render2d.c',
    'src/rng.c',
    'src/spatial.c',
    'src/system.c',
    'src/topaz.c',
    'src/transform.c',
    'src/vector.c',
    'src/wbuffer.c',
]

# Main compiler
cc = meson.get_compiler('c')

# TODO: fix up please!
baselibs = []
baselibs += cc.find_library('m', required : true)
if host_machine.system() == 'windows' or host_machine.system().contains('mingw') == true
    baselibs += cc.find_library('shlwapi')    
endif


# compile args
compile_args = []
if get_option('BuildMode') == 'Debug'
    if cc.get_argument_syntax() == 'gcc'
        compile_args += ['-g',  '-Wall', '-DTOPAZDC_DEBUG']
    endif
    if host_machine.system() != 'windows' and host_machine.system().contains('mingw') == false and host_machine.cpu_family().contains('arm') == false and host_machine.cpu_family().contains('aarch64') == false
        compile_args += ['-fno-omit-frame-pointer', '-fsanitize=address']
    endif
endif 
if get_option('BuildMode') == 'Release'
    compile_args += ['-O2']
endif


# The configuration is meant to wrangle.
# The wrangled calls are wrapped in topaz_config__system_backends, 
# a header included within 'system.c' within topaz.

conf = configuration_data()

# enumeration of handlers
handlers = ''

# include directories
handlerIncludes = ''

# dependency objects
handlerDependencies = []

# enumeration of sources
handlerSources = []

# string of internal C code to be appended to the config header
handlerInternalSource = ''

# string of internal C #includes to be appended to the config header
handlerInternalInclude = ''

# the preferred backends. Must exist within the system/ directory
handlerPreferredAudioManager   = 'audioManager/'   + get_option('AudioManagerBackend')
handlerPreferredInputManager   = 'inputManager/'   + get_option('InputManagerBackend')
handlerPreferredRenderer       = 'renderer/'       + get_option('RendererBackend')
handlerPreferredDisplay        = 'display/'        + get_option('DisplayBackend')
handlerPreferredTime           = 'time/'           + get_option('TimeBackend')
handlerPreferredFilesys        = 'filesys/'        + get_option('FilesystemBackend')
handlerPreferredScript         = 'script/'         + get_option('ScriptBackend')
handlerPreferredConsoleDisplay = 'consoleDisplay/' + get_option('ConsoleDisplayBackend')
handlerPreferredFontRenderer   = 'fontRenderer/'   + get_option('FontRendererBackend')

_allDecs = get_option('IOTranslators')
handlerPreferredIOXs = []
foreach s : _allDecs
    handlerPreferredIOXs += 'iox/' + s
endforeach



subdir('system')



src += handlerSources

conf.set('config_handler_calls',    handlerInternalSource)
conf.set('config_handler_includes', handlerInternalInclude)

configure_file(
    input  : '../../config/config-base.c',
    output : 'topaz_config__system_backends',
    configuration : conf
)

baselibs += handlerDependencies



outputStr = 'COMPILE ARGS: '
foreach s : compile_args
    outputStr += s + ' '
endforeach
message(outputStr)






# modify sources
srcsReal=[]
foreach s : src
    srcsReal += '../../'+s
endforeach



# build the base library
lib = library(
    'topaz',
    srcsReal,
    include_directories : mainIncdir,
    c_args : compile_args,
    link_args : compile_args,
    dependencies : baselibs
)


if get_option('IncludeTests') == true
    message('Making output test executable..')
    test_sources = [
        '../../tests/main.c',
        '../../tests/test.c',
        '../../tests/test_array.c',
        '../../tests/test_entity.c',
        '../../tests/test_string.c',
        '../../tests/test_table.c',
        '../../tests/test_bin.c',
        '../../tests/test_object2d.c'
    ]


    #todo replace with link_lib!

    executable(
        'topaz_test',
        srcsReal + test_sources,
        include_directories : mainIncdir,
        dependencies : baselibs,
        c_args : compile_args,
        link_args : compile_args
    )
endif


# todo: replace with link_lib!
executable(
    'rectangles',
    srcsReal + [
        '../../examples/c/1-Rectangles/main.c', 
        '../../examples/c/1-Rectangles/rectangle.c'
    ],
    include_directories : mainIncdir,
    dependencies : baselibs,
    c_args : compile_args,
    link_args : compile_args
)

executable(
    'scripts',
    srcsReal + [
        '../../examples/c/10-Scripts/main.c'
    ],
    include_directories : mainIncdir,
    dependencies : baselibs,
    c_args : compile_args,
    link_args : compile_args
)


