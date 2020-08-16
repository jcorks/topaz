project(
    'Topaz', 
    'c',
    version : '0.1.0'
)


# This is the root of all includes.
# includes are namespaced properly

mainIncdir = include_directories('include')


# base source files. 
# Add these as needed
src = [
    'src/asset.c',
    'src/assets_image.c',
    'src/backends_audio_manager.c',
    'src/backends_backend.c',
    'src/backends_display.c',
    'src/backends_filesys.c',
    'src/backends_input_manager.c',
    'src/backends_renderer.c',
    'src/backends_time.c',
    'src/camera.c',
    'src/color.c',
    'src/component.c',
    'src/components_scheduler.c',
    'src/components_shape2d.c',
    'src/components_state_control.c',
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
baselibs = cc.find_library('m', required : true)


# The configuration is meant to wrangle.
# The wrangled calls are wrapped in topaz_config__system_backends, 
# a header included within 'system.c' within topaz.

conf = configuration_data()

# enumeration of handlers
handlers = ''

# include directories
handlerIncludes = ''

# library names
handlerLibraries = []

# enumeration of sources
handlerSources = []

# string of internal C code to be appended to the config header
handlerInternalSource = ''

# string of internal C #includes to be appended to the config header
handlerInternalInclude = ''


subdir('system')



src += handlerSources

conf.set('config_handler_calls',    handlerInternalSource)
conf.set('config_handler_includes', handlerInternalInclude)

configure_file(
    input  : 'config/config-base.c',
    output : 'topaz_config__system_backends',
    configuration : conf
)


# build the base library
lib = library(
    'topaz',
    src,
    include_directories : mainIncdir,
    dependencies : baselibs
)



test_sources = [
    'tests/main.c',
    'tests/test.c',
    'tests/test_array.c',
    'tests/test_entity.c',
    'tests/test_string.c',
    'tests/test_table.c'
]
executable(
    'topaz_test',
    test_sources,
    include_directories : mainIncdir,
    dependencies : baselibs,
    link_with : lib


)


