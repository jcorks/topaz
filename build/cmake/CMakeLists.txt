cmake_minimum_required(VERSION 3.10)


project(topaz)

set(CMAKE_C_STANDARD 99)
file(GLOB TOPAZ_SRCS ${PROJECT_SOURCE_DIR}/../../src/*.c)
add_library(libtopaz STATIC ${TOPAZ_SRCS})



set(Backend_AudioManager   "noAudioManager"   CACHE STRING "Audio manager backend for topaz to use and compile support with.")
set(Backend_ConsoleDisplay "noConsoleDisplay" CACHE STRING "Console display backend for topaz to use and compile support with.")
set(Backend_Display        "noDisplay"        CACHE STRING "Display (window) backend for topaz to use and compile support with.")
set(Backend_Filesystem     "noFilesys"        CACHE STRING "Filesystem backend for topaz to use and compile support with.")
set(Backend_FontRenderer   "noFontRenderer"   CACHE STRING "Font renderer backend for topaz to use and compile support with.")
set(Backend_InputManager   "noInputManager"   CACHE STRING "Input manager backend for topaz to use and compile support with.")
set(Backend_IOTranslators  "tpng rawdata"     CACHE STRING "File type translators into topaz assets. Default includes all I/O translators that do not require external or system dependencies.")
set(Backend_Renderer       "noRenderer"       CACHE STRING "Renderer backend for topaz to use and compile support with.")
set(Backend_Script         "gravity"          CACHE STRING "Scripting language engine to embed support for into topaz. Gravity and DuktapeJS do not require any external or system dependencies.")
set(Backend_Time           "noTime"           CACHE STRING "Time tracking backend for topaz to use and copmile support with.")




# Set defaults backends for systems
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("Setting defaults for Windows...")
    set(Backend_ConsoleDisplay "winapi" CACHE STRING "Console display backend for topaz to use and compile support with. Default for Windows: winapi" FORCE)
    set(Backend_Display        "ogl3"   CACHE STRING "Display (window) backend for topaz to use and compile support with. Default for Windows: ogl3" FORCE)
    set(Backend_Filesystem     "winapi" CACHE STRING "Filesystem backend for topaz to use and compile support with. Default for Windows: winapi" FORCE)
    set(Backend_InputManager   "glfw"   CACHE STRING "Input manager backend for topaz to use and compile support with. Default for Windows: glfw" FORCE)
    set(Backend_Renderer       "ogl3"   CACHE STRING "Renderer backend for topaz to use and compile support with. Default for Windows: ogl3" FORCE)
endif()


if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("Setting defaults for Linux...")
    set(Backend_ConsoleDisplay "posix"  CACHE STRING "Console display backend for topaz to use and compile support with. Default for Unix-like: posix" FORCE)
    set(Backend_Display        "ogles2" CACHE STRING "Display (window) backend for topaz to use and compile support with. Default for Unix-like: ogles2" FORCE)
    set(Backend_Filesystem     "posix"  CACHE STRING "Filesystem backend for topaz to use and compile support with. Default for Unix-like: posix" FORCE)
    set(Backend_InputManager   "glfw"   CACHE STRING "Input manager backend for topaz to use and compile support with. Default for Unix-like: glfw" FORCE)
    set(Backend_Renderer       "ogles2" CACHE STRING "Renderer backend for topaz to use and compile support with. Default on Unix-like: ogles2" FORCE)
endif()


set_property(CACHE Backend_AudioManager   PROPERTY STRINGS noAudioManager)
set_property(CACHE Backend_ConsoleDisplay PROPERTY STRINGS noConsoleDisplay posix winapi)
set_property(CACHE Backend_Display        PROPERTY STRINGS ncurses noDisplay ogl3 ogles2)
set_property(CACHE Backend_Filesystem     PROPERTY STRINGS noFilesys posix winapi)
set_property(CACHE Backend_FontRenderer   PROPERTY STRINGS noFontRenderer)
set_property(CACHE Backend_InputManager   PROPERTY STRINGS noInputManager glfw ncurses)
set_property(CACHE Backend_Renderer       PROPERTY STRINGS noRenderer ogl3 ogles2 srgs)
set_property(CACHE Backend_Script         PROPERTY STRINGS noScript gravity duktapeJS)
set_property(CACHE Backend_Time           PROPERTY STRINGS noTime posix)







add_executable(scripts ${PROJECT_SOURCE_DIR}/../../examples/c/10-Scripts/main.c)
target_link_libraries(scripts PUBLIC libtopaz)

target_include_directories(libtopaz PUBLIC ${PROJECT_SOURCE_DIR}/../../include)
target_include_directories(scripts PUBLIC ${PROJECT_SOURCE_DIR}/../../include)

