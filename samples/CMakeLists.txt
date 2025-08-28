function(add_bowstring_sample name)
  add_executable(${name} ${ARGN})

  target_link_libraries(${name} PRIVATE Bowstring)

  set_target_properties(${name} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
  )

  if(MSVC)
    target_compile_options(${name} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endfunction()

add_bowstring_sample(HelloScreen 01-hello-screen.cpp)
