#########
# CMake #
#########

cmake_minimum_required(VERSION 3.11)

project(minecraft-weekend)

set (CMAKE_C_STANDARD 11)

if(EMSCRIPTEN)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin-web)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s TOTAL_MEMORY=1024MB -s FULL_ES3=1")
    # Debug
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ASSERTIONS=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 -s DEMANGLE_SUPPORT=1")
else()
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
endif()

########
# GAME #
########

file(GLOB_RECURSE MAIN_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(game ${MAIN_SRCS})

# Loading resources into Emscripten.
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    file(GLOB_RECURSE files 
        "./res/shaders/*"
        "./res/images/*"
    )

    foreach(file ${files})
        file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} ${file})
        string(APPEND CMAKE_C_FLAGS " --preload-file ${file}@/${relative_file}")
    endforeach()
endif()

##########
# OPENGL #
##########

if(NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)

    target_link_libraries(game OpenGL::GL)
endif()

########
# CGLM #
########

set(CGLM_STATIC ON CACHE INTERNAL "")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cglm)

target_link_libraries(game cglm)

target_include_directories(game
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/cglm/include
)

########
# GLFW #
########

if(NOT EMSCRIPTEN)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)

    target_link_libraries(game glfw)

    target_include_directories(game
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include
    )
endif()

########
# GLAD #
########

add_library(glad STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/src/glad.c
)

target_include_directories(glad
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include
)

target_link_libraries(game glad)

target_include_directories(game
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include
)

#########
# NOISE #
#########

add_library(noise1234 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/noise/noise1234.c)

target_link_libraries(game noise1234)

target_include_directories(game
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/noise
)

#######
# STB #
#######

target_include_directories(game
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb
)