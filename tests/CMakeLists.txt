add_custom_target(tests)
add_dependencies(tests test_lrintf_intrin)

add_executable(test_lrintf_intrin test_lrintf_intrin.c)
target_link_libraries(test_lrintf_intrin PRIVATE gamemath::gamemath)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Looks like linux needs libm linking for the fenv.h functions?
    target_link_libraries(test_lrintf_intrin PRIVATE m)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(GM_TEST_OPTIONS -fno-builtin -fno-strict-aliasing -ffp-contract=off -Wno-implicit-function-declaration)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(GM_TEST_OPTIONS -fno-gnu89-inline -fno-builtin -fno-strict-aliasing -ffp-contract=off -Wno-implicit-function-declaration)
elseif(MSVC)
    set(GM_TEST_OPTIONS /fp:strict /wd4146 /wd4244 /wd4554 /wd4723 /wd4756)
endif()

target_compile_options(test_lrintf_intrin PRIVATE ${GM_TEST_OPTIONS})
