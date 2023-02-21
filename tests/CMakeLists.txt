add_custom_target(tests)
add_dependencies(tests test_lrintf_intrin)

add_executable(test_lrintf_intrin test_lrintf_intrin.c)
target_link_libraries(test_lrintf_intrin PRIVATE gamemath::gamemath)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Looks like linux needs libm linking for the fenv.h functions?
    target_link_libraries(test_lrintf_intrin PRIVATE m)
endif()