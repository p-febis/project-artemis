cmake_minimum_required(VERSION 3.16)
project(Artemis)

add_subdirectory(Bowstring)
add_subdirectory(thirdparty/spdlog)
set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_subdirectory(thirdparty/vk-bootstrap)
set_target_properties(vk-bootstrap PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_SAMPLES "Build Artemis samples" ON)

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
