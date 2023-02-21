cmake_minimum_required(VERSION 3.14)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(gamemath VERSION 1.1.1 LANGUAGES C)

# Decide if this project is the top level project, if so enable development targets.
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

include(FeatureSummary)

# configuration options
option(GM_ENABLE_INTRINSICS "Build using CPU intrinsics where applicatable." ON)
option(GM_ENABLE_TESTS "Build test executables." ${is_top_level})

add_feature_info(EnableIntrinsics GM_ENABLE_INTRINSICS "Use CPU intrinsics where results should match software.")
add_feature_info(EnableTests GM_ENABLE_TESTS "Build test executables.")

if(DEFINED gamemath_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${gamemath_SHARED_LIBS}")
elseif(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
endif()

add_feature_info(BuildSharedLibrary BUILD_SHARED_LIBS "Gamemath will be built as a shared library.")

set(COMMON_SOURCE 
    common/s_signgam.c
    common/infinity.c
)

set(FLOAT_SOURCE
    float/e_acosf.c
    float/e_acoshf.c
    float/e_asinf.c
    float/e_atan2f.c
    float/e_atanhf.c
    float/e_coshf.c
    float/e_expf.c
    float/e_fmodf.c
    float/e_gammaf.c
    float/e_gammaf_r.c
    float/e_hypotf.c
    float/e_j0f.c
    float/e_j1f.c
    float/e_jnf.c
    float/e_lgammaf.c
    float/e_lgammaf_r.c
    float/e_log10f.c
    float/e_log2f.c
    float/e_logf.c
    float/e_powf.c
    float/e_rem_pio2f.c
    float/e_remainderf.c
    float/e_scalbf.c
    float/e_sinhf.c
    float/e_sqrtf.c
    float/k_cosf.c
    float/k_expf.c
    float/k_rem_pio2f.c
    float/k_sinf.c
    float/k_tanf.c
    float/s_asinhf.c
    float/s_atanf.c
    float/s_cbrtf.c
    float/s_ceilf.c
    float/s_copysignf.c
    float/s_cosf.c
    float/s_erff.c
    float/s_exp2f.c
    float/s_expm1f.c
    float/s_fabsf.c
    float/s_finitef.c
    float/s_floorf.c
    float/s_fmaf.c
    float/s_fmaxf.c
    float/s_fminf.c
    float/s_frexpf.c
    float/s_ilogbf.c
    float/s_isfinitef.c
    float/s_isnanf.c
    float/s_isnormalf.c
    float/s_llrintf.c
    float/s_llroundf.c
    float/s_log1pf.c
    float/s_logbf.c
    float/s_lrintf.c
    float/s_lroundf.c
    float/s_modff.c
    float/s_nextafterf.c
    float/s_remquof.c
    float/s_rintf.c
    float/s_roundf.c
    float/s_scalbnf.c
    float/s_significandf.c
    float/s_sinf.c
    float/s_tanf.c
    float/s_tanhf.c
    float/s_truncf.c
    float/w_dremf.c
)

set(DOUBLE_SOURCE
    double/e_acos.c
    double/e_acosh.c
    double/e_asin.c
    double/e_atan2.c
    double/e_atanh.c
    double/e_cosh.c
    double/e_exp.c
    double/e_fmod.c
    double/e_gamma.c
    double/e_gamma_r.c
    double/e_hypot.c
    double/e_j0.c
    double/e_lgamma.c
    double/e_lgamma_r.c
    double/e_log.c
    double/e_log10.c
    double/e_log2.c
    double/e_pow.c
    double/e_rem_pio2.c
    double/e_remainder.c
    double/e_scalb.c
    double/e_sinh.c
    double/e_sqrt.c
    double/k_cos.c
    double/k_exp.c
    double/k_rem_pio2.c
    double/k_sin.c
    double/k_tan.c
    double/s_asinh.c
    double/s_atan.c
    double/s_cbrt.c
    double/s_ceil.c
    double/s_copysign.c
    double/s_cos.c
    double/s_erf.c
    double/s_exp2.c
    double/s_expm1.c
    double/s_fabs.c
    double/s_floor.c
    double/s_finite.c
    double/s_fma.c
    double/s_fmax.c
    double/s_fmin.c
    double/s_frexp.c
    double/s_ilogb.c
    double/s_isfinite.c
    double/s_isnan.c
    double/s_isnormal.c
    double/s_llrint.c
    double/s_log1p.c
    double/s_logb.c
    double/s_lrint.c
    double/s_lround.c
    double/s_modf.c
    double/s_nextafter.c
    double/s_remquo.c
    double/s_rint.c
    double/s_round.c
    double/s_scalbn.c
    double/s_significand.c
    double/s_sin.c
    double/s_tan.c
    double/s_tanh.c
    double/s_trunc.c
    double/w_drem.c
)

if(CMAKE_CONFIGURATION_TYPES)
    # Glob all the header files together to add to the project for msvc/xcode.
    # Not ideal as CMake won't notice if you add any until something else prompts a CMake run
    # but this is largely a convenience feature for MSVC/XCode users.
    file(GLOB_RECURSE GMATH_HEADERS "*.h")
    file(GLOB_RECURSE GMATH_SOURCES "*.c")

    foreach(_GMATH_SOURCE IN ITEMS ${GMATH_SOURCES})
        get_filename_component(_GMATH_SOURCE_PATH "${_GMATH_SOURCE}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" _GAMEENGINE_GROUP_PATH "${_GMATH_SOURCE_PATH}")
        string(REPLACE "/" "\\" _GAMEENGINE_GROUP_PATH "${_GAMEENGINE_GROUP_PATH}")
        source_group("Source Files\\${_GAMEENGINE_GROUP_PATH}" FILES "${_GMATH_SOURCE}")
    endforeach()

    foreach(_GMATH_HEADER IN ITEMS ${GMATH_HEADERS})
        get_filename_component(_GMATH_HEADER_PATH "${_GMATH_HEADER}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" _GAMEENGINE_HGROUP_PATH "${_GMATH_HEADER_PATH}")
        string(REPLACE "/" "\\" _GAMEENGINE_HGROUP_PATH "${_GAMEENGINE_HGROUP_PATH}")
        source_group("Header Files\\${_GAMEENGINE_HGROUP_PATH}" FILES "${_GMATH_HEADER}")
    endforeach()
endif()

add_library(gamemath ${COMMON_SOURCE} ${FLOAT_SOURCE} ${DOUBLE_SOURCE} ${GMATH_HEADERS})
add_library(gamemath::gamemath ALIAS gamemath)

target_include_directories(gamemath PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE common 
)

target_compile_features(gamemath PRIVATE c_std_99)

if(BUILD_SHARED_LIBS)
    set_target_properties(gamemath PROPERTIES ARCHIVE_OUTPUT_NAME gamemath_import SOVERSION 1.1 VERSION 1.1.1 WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(gamemath PRIVATE -fno-builtin -fno-strict-aliasing -Wno-implicit-function-declaration)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    target_compile_options(gamemath PRIVATE -fno-gnu89-inline -fno-builtin -fno-strict-aliasing -Wno-implicit-function-declaration)
elseif(MSVC)
    target_compile_options(gamemath PRIVATE /wd4146 /wd4244 /wd4554 /wd4723 /wd4756)
endif()

set_target_properties(gamemath PROPERTIES OUTPUT_NAME gm)
set_target_properties(gamemath PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS} OUTPUT_NAME gm)

if(GM_ENABLE_INTRINSICS)
    target_compile_definitions(gamemath PRIVATE BUILD_WITH_INTRINSICS)
endif()

if(GM_ENABLE_TESTS)
    # Tests need to respect some options so need to occur after the options are set.
    enable_testing()
    add_subdirectory(tests)
endif()

feature_summary(WHAT ENABLED_FEATURES DESCRIPTION "Enabled features:")
feature_summary(WHAT DISABLED_FEATURES DESCRIPTION "Disabled features:")
