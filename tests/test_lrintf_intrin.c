#include <fenv.h>
#include <gmath.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#if defined _WIN32 && (defined _M_IX86 || defined _M_AMD64)
#include <intrin.h>
#define USE_SSE 1
#elif (defined __clang__ || defined __GNUC__) && (defined __i386__ || defined __amd64__)
#include <x86intrin.h>
#define USE_SSE 1
#endif

#ifdef USE_SSE
long int intrin_lrintf(float x)
{
    return _mm_cvt_ss2si(_mm_load_ss(&x));
}
#endif

long int test_lrintf(float x)
{
    fenv_t env;
    long int d;

    feholdexcept(&env);
    d = (long int)gm_rintf(x);
    if (fetestexcept(FE_INVALID))
        feclearexcept(FE_INEXACT);
    feupdateenv(&env);
    return (d);
}

#if defined(__GNUC__) || defined(__clang__)
/*
 * GCC/Clang specific syntax to allow violation of strict aliasing rules.
 */
typedef union
{
    float __attribute__((__may_alias__)) value;
    uint32_t __attribute__((__may_alias__)) word;
} ieee_float_shape_type;
#else
typedef union
{
    float value;
    uint32_t word;
} ieee_float_shape_type;
#endif

int test_range(uint32_t start, uint32_t stop)
{
    int ret = 0;
    printf("Testing from %u to %u (inclusive).\n", start, stop);
    uint32_t i = start;
    while (i <= stop)
    {
        ieee_float_shape_type input;
        input.word = i;
        long soft_result = test_lrintf(input.value);
        long ref_result = lrintf(input.value);
        // If the results don’t match then report an error.

        if (soft_result != ref_result)
        {
            ret = 1;
            printf("Input %.9g, expected %ld, got %ld from software implementation\n",
                input.value, soft_result, ref_result);
        }

#ifdef USE_SSE
        long intrin_result = intrin_lrintf(input.value);
        if (intrin_result != ref_result)
        {
            ret = 1;
            printf("Input %.9g, expected %ld, got %ld from sse implementation\n",
                input.value, soft_result, ref_result);
        }
#endif

        ++i;
    }

    return ret;
}

int main(int argc, char** argv)
{
    int ret = 0;

    ret |= test_range(0, 0xFFFFFFFF);

    return ret;
}
