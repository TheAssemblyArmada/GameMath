cmake_minimum_required(VERSION 3.1.0)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

project(gamemath VERSION 1.1.0 LANGUAGES C)

if(MSVC)
    # Prevent incremental linking.
    string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" replacementFlags ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DYNAMICBASE:NO /NXCOMPAT:NO /INCREMENTAL:NO ${replacementFlags}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/DYNAMICBASE:NO /NXCOMPAT:NO /INCREMENTAL:NO ${replacementFlags}")

    string(REPLACE "INCREMENTAL" "INCREMENTAL:NO" replacementFlags ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO})
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO ${replacementFlags}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO ${replacementFlags}")
    
    # Disable Run Time Checking.
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
        #message("Processing flags ${flag_var}")
        string(REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
endif()

# configuration options
option(GM_ENABLE_STATIC "Build static gamemath library." ON)
option(GM_ENABLE_SHARED "Build shared gamemath library." OFF)
if(NOT GM_ENABLE_STATIC AND NOT GM_ENABLE_SHARED)
    set(GM_ENABLE_STATIC ON)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fno-builtin -fno-strict-aliasing -Wno-implicit-function-declaration")
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fno-gnu89-inline -fno-builtin -Wno-implicit-function-declaration")
endif()

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

if(NOT DEFINED gamemath_USE_OBJECT_LIB)
    if(GM_ENABLE_STATIC AND GM_ENABLE_SHARED)
        if(CMAKE_POSITION_INDEPENDENT_CODE OR MSVC)
            # all objects are PIC on this system => can share objects
            set(gamemath_USE_OBJECT_LIB 1)
        endif()
    endif()
endif()

if(gamemath_USE_OBJECT_LIB)
    add_library(gamemath_object_lib OBJECT ${COMMON_SOURCE} ${FLOAT_SOURCE} ${DOUBLE_SOURCE} ${GMATH_HEADERS})
    set_target_properties(gamemath_object_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_include_directories(gamemath_object_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE common 
    )
    target_compile_features(gamemath_object_lib PRIVATE c_std_99)
endif()

if(GM_ENABLE_STATIC)
    if(gamemath_USE_OBJECT_LIB)
        add_library(gamemath_static_lib STATIC $<TARGET_OBJECTS:gamemath_object_lib>)
    else()
        add_library(gamemath_static_lib STATIC ${COMMON_SOURCE} ${FLOAT_SOURCE} ${DOUBLE_SOURCE} ${GMATH_HEADERS})
    endif()
    
    set_target_properties(gamemath_static_lib PROPERTIES OUTPUT_NAME gm)
    target_include_directories(gamemath_static_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE common 
    )
    target_compile_features(gamemath_static_lib PRIVATE c_std_99)
endif()

if(GM_ENABLE_SHARED)
    if(gamemath_USE_OBJECT_LIB)
        add_library(gamemath_shared_lib SHARED $<TARGET_OBJECTS:gamemath_object_lib>)
    else()
        add_library(gamemath_shared_lib SHARED ${COMMON_SOURCE} ${FLOAT_SOURCE} ${DOUBLE_SOURCE} ${GMATH_HEADERS})
    endif()
    set_target_properties(gamemath_shared_lib PROPERTIES OUTPUT_NAME gm)
    if(GM_ENABLE_STATIC)
        # avoid name clash between static gm.lib and DLL import-lib gm.lib
        set_target_properties(gamemath_shared_lib PROPERTIES ARCHIVE_OUTPUT_NAME gamemath_dllimport)
    endif()
    set_target_properties(gamemath_shared_lib PROPERTIES SOVERSION 1.1 VERSION 1.1.0) # ABI version is 1.1.0 !
    set_target_properties(gamemath_shared_lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_include_directories(gamemath_static_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE common 
    )
    target_compile_features(gamemath_shared_lib PRIVATE c_std_99)
endif()
