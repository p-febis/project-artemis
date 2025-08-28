add_library(Bowstring INTERFACE)

find_package(Vulkan REQUIRED)

function(add_bowstring_module name)
  add_library(Bowstring_${name} SHARED ${ARGN})

  target_include_directories(Bowstring_${name}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
  )

  target_compile_features(Bowstring_${name} PUBLIC cxx_std_17)

  if(MSVC)
    target_compile_options(Bowstring_${name} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(Bowstring_${name} PRIVATE -Wall -Wextra -Wpedantic)
  endif()

  target_link_libraries(Bowstring INTERFACE Bowstring_${name})
endfunction()


add_bowstring_module(
  Logging
  src/Bowstring/Logging.h
  src/Logging.cpp
)
target_link_libraries(Bowstring_Logging PUBLIC spdlog)


add_bowstring_module(
  Window
  src/Bowstring/Window.h
  src/Window.cpp
)
target_link_libraries(Bowstring_Window PUBLIC Bowstring_Logging glfw)

add_bowstring_module(
  Renderer
  src/Bowstring/Renderer.h
  src/Renderer.cpp
)
target_link_libraries(Bowstring_Renderer PUBLIC Bowstring_Window vk-bootstrap::vk-bootstrap Vulkan::Vulkan)

add_bowstring_module(
  Application
  src/Bowstring/Application.h
  src/Application.cpp
)
target_link_libraries(Bowstring_Application PUBLIC Bowstring_Renderer glm)
