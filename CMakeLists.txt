# This CMake script is designed for CLion IDE and QTCreator projects on Windows
# It should find bundled binary dependencies automatically when using MinGW compiler
# On Linux and Mac the dependencies (glm, glew, glfw) need to be installed manually

cmake_minimum_required(VERSION 3.1)
project(PPGSOHerna)

#
# CONFIGURATION
#

# Basic CMake settings
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data)

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# Use add_resources function to convert .glsl to a .cpp files which will be compiled into a separate static library
include(add_resources)

#
# DEPENDENCIES
#

# Set up external dependencies for Windows users
if (MINGW)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/include/")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/dependencies/lib/mingw")
elseif (MSVC)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/include/")
    if(MSVC_VERSION == 1500)
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/dependencies/lib/vc2015")
    elseif(MSVC_VERSION >= 1900)
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/dependencies/lib/vc2019")
    endif()
    set(USE_STRICT_COMPILE_WARNINGS OFF CACHE BOOL "" FORCE)
endif ()

# Warnings for Debug mode
option(USE_STRICT_COMPILE_WARNINGS "Use strict compilation warnings in debug mode." ON)
# These compile flags should apply for GCC and Clang compilers
set(STRICT_COMPILE_FLAGS "-Wpedantic -Wall -Wno-c++98-compat -Wextra -Wno-sign-conversion -Wno-unused-parameter")
if (USE_STRICT_COMPILE_WARNINGS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${STRICT_COMPILE_FLAGS}")
endif ()

# Find required packages
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)

# Optional packages
find_package(OpenMP)
if(OPENMP_FOUND)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
endif()

# Set default installation destination
if (NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "../_install")
endif ()

#
# Shaders / Materials
#

set(PPGSO_SHADER_SRC
        shader/color_vert.glsl shader/color_frag.glsl
        shader/convolution_vert.glsl shader/convolution_frag.glsl
        shader/diffuse_vert.glsl shader/diffuse_frag.glsl
        shader/texture_vert.glsl shader/texture_frag.glsl
)
add_resources(shaders ${PPGSO_SHADER_SRC})

# PPGSO library
add_library(ppgso STATIC
        ppgso/mesh.cpp
        ppgso/tiny_obj_loader.cpp
        ppgso/shader.cpp
        ppgso/image.cpp
        ppgso/image_bmp.cpp
        ppgso/image_raw.cpp
        ppgso/texture.cpp
        ppgso/window.cpp
)

# Make sure GLM uses radians and GLEW is a static library
target_compile_definitions(ppgso PUBLIC -DGLM_FORCE_RADIANS -DGLEW_STATIC)

# Link to GLFW, GLEW and OpenGL
target_link_libraries(ppgso PUBLIC ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
# Pass on include directories
target_include_directories(ppgso PUBLIC
        ppgso
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR})

#
# TARGETS
#

# main
add_executable(Herna
        src/main.cpp

        src/scene/Scene.cpp

        src/object/Object.cpp
        src/object/StaticObject.cpp

        src/camera/Camera.cpp
        src/camera/DebugCamera.cpp
)
target_link_libraries(Herna ppgso shaders)
install(TARGETS Herna DESTINATION .)
add_custom_command(TARGET Herna POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_CURRENT_BINARY_DIR})

# main_kocalka
add_executable(HernaKocalka
        src/main_kocalka.cpp

        src/scene/Scene.cpp

        src/object/Object.cpp
        src/object/StaticObject.cpp
        src/object/VirtualObject.cpp
        src/object/Room.cpp
        src/object/Arcade.cpp
        src/object/Billiard.cpp
        src/object/Dartboard.cpp
        src/object/Chandelier.cpp
        src/object/Glass.cpp

        src/camera/Camera.cpp
        src/camera/OrthoCamera.cpp
        src/camera/PersCamera.cpp
        src/camera/DebugCamera.cpp
        src/camera/ShadowMap.cpp

        src/object/BilliardBall.cpp
        src/object/BilliardBall.h
        src/object/BilliardWall.cpp
        src/object/BilliardWall.h
        src/object/CollisionGroup.cpp
        src/object/CollisionGroup.h
        src/object/ObjectResources.cpp
        src/object/ObjectResources.h
)
target_link_libraries(HernaKocalka ppgso shaders)
install(TARGETS HernaKocalka DESTINATION .)
add_custom_command(TARGET HernaKocalka POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_CURRENT_BINARY_DIR})

# main_dmitriev
add_executable(HernaDmitriev
        src/main_dmitriev.cpp

        src/scene/Scene.cpp

        src/object/Object.cpp
        src/object/StaticObject.cpp
        src/object/Room.cpp
        src/object/Arcade.cpp
        src/object/Billiard.cpp
        src/object/Dartboard.cpp
        src/object/Chandelier.cpp
        src/object/Glass.cpp

        src/camera/Camera.cpp
        src/camera/DebugCamera.cpp

        src/object/BilliardBall.cpp
        src/object/BilliardBall.h
        src/object/BilliardWall.cpp
        src/object/BilliardWall.h
        src/object/CollisionGroup.cpp
        src/object/CollisionGroup.h
)
target_link_libraries(HernaDmitriev ppgso shaders)

install(TARGETS HernaDmitriev DESTINATION .)
add_custom_command(TARGET HernaDmitriev POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_CURRENT_BINARY_DIR})

#
# INSTALLATION
#

install(DIRECTORY data/ DESTINATION .)



