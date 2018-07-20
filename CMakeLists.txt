cmake_minimum_required(VERSION 3.0.0)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(PROJECT gamemath VERSION 1.0.0 LANGUAGES C)

project(${PROJECT})

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
    float/e_logf.c
    float/e_powf.c
    float/e_rem_pio2f.c
    float/e_remainderf.c
    float/e_scalbf.c
    float/e_sinhf.c
    float/e_sqrtf.c
    float/k_cosf.c
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
    float/s_isnanf.c
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
    # TODO
)

if(NOT DEFINED gamemath_USE_OBJECT_LIB)
    if(GM_ENABLE_STATIC AND GM_ENABLE_SHARED)
        if(CMAKE_POSITION_INDEPENDENT_CODE OR MSVC)
            # all objects are PIC on this system => can share objects
            set(gamemath_USE_OBJECT_LIB 1)
        endif()
    endif()
endif()

if(gamemath_USE_OBJECT_LIB)
    add_library(gamemath_object_lib OBJECT ${COMMON_SOURCE} ${FLOAT_SOURCE} ${DOUBLE_SOURCE})
    set_property(TARGET gamemath_object_lib PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(gamemath_object_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE common 
    )
endif()

if(GM_ENABLE_STATIC)
    if(gamemath_USE_OBJECT_LIB)
        add_library(gamemath_static_lib STATIC $<TARGET_OBJECTS:gamemath_object_lib>)
    else()
        add_library(gamemath_static_lib STATIC ${COMMON_SOURCE} ${FLOAT_SOURCE} ${DOUBLE_SOURCE})
    endif()
    
    set_target_properties(gamemath_static_lib PROPERTIES OUTPUT_NAME gm)
    target_include_directories(gamemath_static_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE common 
    )
endif()

if(GM_ENABLE_SHARED)
    if(gamemath_USE_OBJECT_LIB)
        add_library(gamemath_shared_lib SHARED $<TARGET_OBJECTS:gamemath_object_lib>)
    else()
        add_library(gamemath_shared_lib SHARED ${COMMON_SOURCE} ${FLOAT_SOURCE} ${DOUBLE_SOURCE})
    endif()
    set_target_properties(gamemath_shared_lib PROPERTIES OUTPUT_NAME gm)
    if(GM_ENABLE_STATIC)
        # avoid name clash between static gm.lib and DLL import-lib gm.lib
        set_target_properties(gamemath_shared_lib PROPERTIES ARCHIVE_OUTPUT_NAME gamemath_dllimport)
    endif()
    set_target_properties(gamemath_shared_lib PROPERTIES SOVERSION 1.0 VERSION 1.0.0) # ABI version is 1.0.0 !
    set_target_properties(gamemath_shared_lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_include_directories(gamemath_static_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE common 
    )
endif()
