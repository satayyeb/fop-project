set(SOURCE src/main.c)
set(PROJECT_NAME island_soldier)
cmake_minimum_required(VERSION 3.19)
#cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME} C)
set(CMAKE_C_STANDARD 11)


set(SDL2_INCLUDE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-include")
set(SDL2_LIB_DIR         "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-lib")

set(SDL2_GFX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-gfx-include")
set(SDL2_GFX_LIB_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-gfx-lib")

set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-image-include")
set(SDL2_IMAGE_LIB_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-image-lib")

set(SDL2_TTF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-ttf-include")
set(SDL2_TTF_LIB_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/sdl2/sdl2-ttf-lib")


set(SDL2_FLAGS   "-mwindows -Wl,--no-undefined -static-libgcc")


add_library(SDL2            STATIC IMPORTED)
add_library(SDL2main        STATIC IMPORTED)

add_library(SDL2_GFX        STATIC IMPORTED)

add_library(SDL2_IMAGE_DLL  STATIC IMPORTED)

add_library(SDL2_TTF_DLL    STATIC IMPORTED)



set_property(TARGET SDL2            PROPERTY IMPORTED_LOCATION "${SDL2_LIB_DIR}/libSDL2.a")
set_property(TARGET SDL2main        PROPERTY IMPORTED_LOCATION "${SDL2_LIB_DIR}/libSDL2main.a")

set_property(TARGET SDL2_GFX        PROPERTY IMPORTED_LOCATION "${SDL2_GFX_LIB_DIR}/libsdl-gfx.a")

set_property(TARGET SDL2_IMAGE_DLL  PROPERTY IMPORTED_LOCATION "${SDL2_IMAGE_LIB_DIR}/libSDL2_image.dll.a")

set_property(TARGET SDL2_TTF_DLL    PROPERTY IMPORTED_LOCATION "${SDL2_TTF_LIB_DIR}/libSDL2_ttf.dll.a")



set(SDL2_LIBS mingw32 SDL2main SDL2 SDL2_GFX SDL2_IMAGE_DLL SDL2_TTF_DLL m dinput8 dxguid dxerr8 user32 gdi32 winmm imm32 ole32 oleaut32 shell32 version uuid)
#set(SDL2_LIBS SDL2 SDL2main SDL2_GFX SDL2_TTF_DLL SDL2_IMAGE_DLL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${SDL2_FLAGS}")
file(GLOB_RECURSE SOURCE "src/*.c" "src/*.h")


#IF(WIN32)
#    SET(GUI_TYPE WIN32)
#ENDIF(WIN32)
#IF (APPLE)
#    SET(GUI_TYPE MACOSX_BUNDLE)
#ENDIF (APPLE)

add_executable("${PROJECT_NAME}" ${GUI_TYPE} "${SOURCE}")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBS})

