# built in support for the following system backends:
#
# AudioManager -> WASAPI
# Display -> OpenGL3+
# Filesystem -> WINAPI
# InputManager -> GLFW
# Renderer -> OpenGL3+
# Time -> WINAPI
# Scripting -> DuktapeJS
# ConsoleDisplay -> WINAPI
# FontRenderer -> Freetype
# IOTranslators:
#	rawdata
#	tPNG
#	OGG
#
CC = gcc
SRCS := \
    src/asset.c\
    src/assets_image.c\
    src/assets_data.c\
    src/assets_sound.c\
    src/assets_material.c\
    src/assets_mesh.c\
    src/backends_audio_manager.c\
    src/backends_console_display.c\
    src/backends_display.c\
    src/backends_filesystem.c\
    src/backends_font_renderer.c\
    src/backends_input_manager.c\
    src/backends_renderer.c\
    src/backends_time.c\
    src/backends_script.c\
    src/backends_iox.c\
    src/camera.c\
    src/color.c\
    src/component.c\
    src/components_scheduler.c\
    src/components_shape2d.c\
    src/components_shape3d.c\
    src/components_state_control.c\
    src/components_object2d.c\
    src/components_text2d.c\
    src/components_automation.c\
    src/containers_array.c\
    src/containers_bin.c\
    src/containers_string.c\
    src/containers_table.c\
    src/entity.c\
    src/input_device.c\
    src/math.c\
    src/matrix.c\
    src/modules_input.c\
    src/modules_resources.c\
    src/modules_view_manager.c\
    src/modules_graphics.c\
    src/modules_script_manager.c\
    src/modules_font_manager.c\
    src/modules_console.c\
    src/modules_audio.c\
    src/particle.c\
    src/rbuffer.c\
    src/refbank.c\
    src/render2d.c\
    src/rng.c\
    src/spatial.c\
    src/system.c\
    src/topaz.c\
    src/transform.c\
    src/vector.c\
    src/wbuffer.c\


SRCS += system/audioManager/wasapi/src/main.c
SRCS += system/consoleDisplay/winapi/src/main.c
SRCS += system/display/ogl3/src/main.c
SRCS += system/filesystem/winapi/src/winapi.c
SRCS += system/fontRenderer/freetype/src/main.c
SRCS += system/inputManager/glfw/src/main.c
SRCS += system/iox/ogg/src/main.c
SRCS += system/iox/rawdata/src/main.c
SRCS += system/iox/tpng/src/main.c
SRCS += system/iox/tpng/src/tpng.c
SRCS +=  system/renderer/ogl3/src/main.c\
         system/renderer/ogl3/src/buffer.c\
         system/renderer/ogl3/src/charpixel.c\
         system/renderer/ogl3/src/program2d.c\
         system/renderer/ogl3/src/2d.c\
         system/renderer/ogl3/src/texture.c\
         system/renderer/ogl3/src/framebuffer.c\
         system/renderer/ogl3/src/context.c\
         system/renderer/ogl3/src/program.c \
		 system/renderer/ogl3/src/glad.c

SRCS += system/script/duktapeJS/src/main.c\
        system/script/duktapeJS/src/duktape.c\
        system/script/duktapeJS/src/duk_trans_dvalue.c\

SRCS += system/time/winapi/src/time_winapi_topaz.c

SRCS := $(addprefix ../../../,$(SRCS))
OBJS := $(SRCS:.c=.o)


INCS := -I../../../include/ -I. -I/mingw64/include/freetype2/





FLAGS = -static 
LIBS :=  -lvorbisfile  -lwinmm -lvorbis -logg -lharfbuzz -lfreetype -lharfbuzz -lfreetype -lglfw3 -lOpenGL32 -lshlwapi  -lusp10 -lgraphite2 -lstdc++ -lpng -lGDI32 -lz  -lbz2 -lrpcrt4 -lole32 -lbrotlidec-static -lbrotlicommon-static


%.o: %.c 
	$(CC) -c $(FLAGS) $(DEFS) $(INCS) $< -o $@

all: $(OBJS)
	@echo linking 
	$(CC) -O2 $(FLAGS) $(OBJS) $(INCS) ../../../examples/c/10-Scripts/main.c -o ./scripts $(LIBS) 

debug: $(OBJS)
	$(CC) -g $(FLAGS) $(OBJS) $(INCS) ../../../examples/c/10-Scripts/main.c -o ./scripts $(LIBS) 

clean: 
	rm -f $(OBJS) ./scripts