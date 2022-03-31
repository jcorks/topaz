# While convenient, the regular makefiles are more accessible 
# CMake is kind of a mess
cmake_minimum_required(VERSION 3.10)


project(topaz)

set(CMAKE_C_STANDARD 99)
file(GLOB TOPAZ_SRCS ${PROJECT_SOURCE_DIR}/../../src/*.c)

file(GLOB TOPAZ_SRCS__BACKEND 
    ${PROJECT_SOURCE_DIR}/../../system/audioManager/${Backend_AudioManager}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/consoleDisplay/${Backend_ConsoleDisplay}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/display/${Backend_Display}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/filesystem/${Backend_Filesystem}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/fontRenderer/${Backend_FontRenderer}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/inputManager/${Backend_InputManager}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/renderer/${Backend_Renderer}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/script/matte/src/matte/src/rom/native.c 
    ${PROJECT_SOURCE_DIR}/../../system/script/matte/src/matte/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/script/matte/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/time/${Backend_Time}/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/iox/tpng/src/*.c 
    ${PROJECT_SOURCE_DIR}/../../system/iox/rawdata/src/*.c
)

message(${TOPAZ_SRCS__BACKEND})

add_executable(topaz ${TOPAZ_SRCS} ${TOPAZ_SRCS__BACKEND} ${PROJECT_SOURCE_DIR}/../../src/binary/main.c)
add_library(libtopaz STATIC ${TOPAZ_SRCS} ${TOPAZ_SRCS__BACKEND})




set(Backend_AudioManager   "noAudioManager"   CACHE STRING "Audio manager backend for topaz to use and compile support with.")
set(Backend_ConsoleDisplay "noConsoleDisplay" CACHE STRING "Console display backend for topaz to use and compile support with.")
set(Backend_Display        "noDisplay"        CACHE STRING "Display (window) backend for topaz to use and compile support with.")
set(Backend_Filesystem     "noFilesys"        CACHE STRING "Filesystem backend for topaz to use and compile support with.")
set(Backend_FontRenderer   "noFontRenderer"   CACHE STRING "Font renderer backend for topaz to use and compile support with.")
set(Backend_InputManager   "noInputManager"   CACHE STRING "Input manager backend for topaz to use and compile support with.")
set(Backend_IOTranslators  "tpng rawdata"     CACHE STRING "File type translators into topaz assets. Default includes all I/O translators that do not require external or system dependencies.")
set(Backend_Renderer       "noRenderer"       CACHE STRING "Renderer backend for topaz to use and compile support with.")
set(Backend_Time           "noTime"           CACHE STRING "Time tracking backend for topaz to use and copmile support with.")




# Set defaults backends for systems
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("Setting defaults for Windows...")
    set(Backend_ConsoleDisplay "winapi"   CACHE STRING "Console display backend for topaz to use and compile support with. Default for Windows: winapi" FORCE)
    set(Backend_Display        "ogl3"     CACHE STRING "Display (window) backend for topaz to use and compile support with. Default for Windows: ogl3" FORCE)
    set(Backend_Filesystem     "winapi"   CACHE STRING "Filesystem backend for topaz to use and compile support with. Default for Windows: winapi" FORCE)
    set(Backend_InputManager   "glfw"     CACHE STRING "Input manager backend for topaz to use and compile support with. Default for Windows: glfw" FORCE)
    set(Backend_Renderer       "ogl3"     CACHE STRING "Renderer backend for topaz to use and compile support with. Default for Windows: ogl3" FORCE)
    set(Backend_Time           "winapi"   CACHE STRING "Type backend for topaz to use and compile support with. Default for Windows: winapi" FORCE)
    set(Backend_AudioManager   "wasapi"   CACHE STRING "Audio manager backend for topaz to use and compile support with. Default for Windows: wasapi" FORCE)
    set(Backend_FontRenderer   "freetype" CACHE STRING "Font renderer backend for topaz to use and compile support with. Default is: Freetype")
endif()


if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("Setting defaults for Linux...")
    set(Backend_ConsoleDisplay "posix"  CACHE STRING "Console display backend for topaz to use and compile support with. Default for Unix-like: posix" FORCE)
    set(Backend_Display        "ogl3"   CACHE STRING "Display (window) backend for topaz to use and compile support with. Default for Unix-like: ogles2" FORCE)
    set(Backend_Filesystem     "posix"  CACHE STRING "Filesystem backend for topaz to use and compile support with. Default for Unix-like: posix" FORCE)
    set(Backend_InputManager   "glfw"   CACHE STRING "Input manager backend for topaz to use and compile support with. Default for Unix-like: glfw" FORCE)
    set(Backend_Renderer       "ogl3"   CACHE STRING "Renderer backend for topaz to use and compile support with. Default on Unix-like: ogles2" FORCE)
    set(Backend_Time           "posix"  CACHE STRING "Type backend for topaz to use and compile support with. Default for Linux: posix" FORCE)
    set(Backend_AudioManager   "alsa"   CACHE STRING "Audio manager backend for topaz to use and compile support with. Default for Linux: alsa" FORCE)
    set(Backend_FontRenderer   "freetype" CACHE STRING "Font renderer backend for topaz to use and compile support with. Default is: Freetype")
endif()


set_property(CACHE Backend_AudioManager   PROPERTY STRINGS noAudioManager)
set_property(CACHE Backend_ConsoleDisplay PROPERTY STRINGS noConsoleDisplay posix winapi)
set_property(CACHE Backend_Display        PROPERTY STRINGS ncurses noDisplay ogl3 ogles2)
set_property(CACHE Backend_Filesystem     PROPERTY STRINGS noFilesys posix winapi)
set_property(CACHE Backend_FontRenderer   PROPERTY STRINGS noFontRenderer freetype)
set_property(CACHE Backend_InputManager   PROPERTY STRINGS noInputManager glfw ncurses)
set_property(CACHE Backend_Renderer       PROPERTY STRINGS noRenderer ogl3 ogles2 srgs)
set_property(CACHE Backend_Time           PROPERTY STRINGS noTime posix)


if (${Backend_AudioManager} STREQUAL "alsa")
    find_package(ALSA REQUIRED)
    if (ALSA_FOUND)                                                
        include_directories(${ALSA_INCLUDE_DIRS})                  
        target_link_libraries(topaz ${ALSA_LIBRARIES})    
    else()
        message("NO ALSA")
    endif(ALSA_FOUND)
endif()

if (${Backend_Display} STREQUAL "ogl3")
    find_package(OpenGL REQUIRED)
    if (OpenGL_FOUND)                                                
        include_directories(${OpenGL_INCLUDE_DIRS})                  
        target_link_libraries(topaz ${OpenGL_LIBRARIES})      
    endif(OpenGL_FOUND)

    find_package(glfw3 REQUIRED)
    target_link_libraries(topaz glfw)      

endif()

if (${Backend_InputManager} STREQUAL "glfw")
    find_package(glfw3 REQUIRED)
    target_link_libraries(topaz glfw)      
endif()

if (${Backend_Renderer} STREQUAL "ogl3")
    find_package(OpenGL REQUIRED)
    if (OpenGL_FOUND)                                                
        include_directories(${OpenGL_INCLUDE_DIRS})                  
        target_link_libraries(topaz ${OpenGL_LIBRARIES})      
    endif(OpenGL_FOUND)
endif()

if (${Backend_Renderer} STREQUAL "freetype")
    find_package(freetype2 REQUIRED)
    if (freetype2_FOUND)                                                
        include_directories(${freetype2_INCLUDE_DIRS})                  
        target_link_libraries(topaz ${freetype2_LIBRARIES})      
    endif(freetype2_FOUND)
endif()

configure_file(${PROJECT_SOURCE_DIR}/topaz_config__system_backends_INPUT ${PROJECT_SOURCE_DIR}/topaz_config__system_backends)


target_link_libraries(topaz m dl pthread)
target_link_libraries(libtopaz m pthread)

target_include_directories(libtopaz PUBLIC ${PROJECT_SOURCE_DIR}/../../include ${PROJECT_SOURCE_DIR}/)
target_include_directories(topaz PUBLIC ${PROJECT_SOURCE_DIR}/../../include ${PROJECT_SOURCE_DIR}/)


get_target_property(OUT topaz LINK_LIBRARIES)
message(STATUS ${OUT})
