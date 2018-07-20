# GameMath

GameMath is a C mathmatical library derived from the FreeBSD msun library. The idea is to have a self contained floating point math library that will behave consistently on IEEE754 conformant systems making floating point operations reproducible across compilers and hardware.

This is needed for games where the network and replay code relies on clients maintaining their own simulation state (so called lock step simulation) and only pass lists of commands between clients. Without consistent floating point results at the binary level the simulations can diverge and go out of sync.

Relying on the platform libc math libraries is generally fraught with issues. Some call the CPU trancedental instructions where available which can produce different results on different CPU models. Even where this isn't the case different versions of the libraries can produce different results between updates on the same platform and are rarely consistent between platforms.

By using a common software library for these operations we can remove one source of inconsistency.

## Usage

The recommended way to use GameMath is to just include it in your source tree and add it as a subdirectory in cmake. You can then link your main binary against the gamemath_static_lib target and include gmath.h where you would normally include math.h or cmath.

The GameMath versions of the standard C maths functions are prefixed with gm_ so you would use gm_tanf rather than tanf. Currently only the float precision versions of the functions are implemented.

GameMath will only get you so far in getting consistent floating point results and may not give the same accuracy as the libc implementations on any given platform. We are generally more interested in consistent results than accurate results from a game networking perspective. The [SEI CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87151969) highlights some additional issues that should be considered with floating point values and ways to mitigate them.

It is recommended that where possible your compiler is configured to give FLT_EVAL_METHOD == 0. On x86 platforms this is normally achieved by enabling SSE2 code generation, even though most of the float precision implementations in GameMath use float precision exclusively and should only require SSE.

## Alternatives

When searching for floating point maths libraries you may come across openlim or streflop. Of the two only streflop uses pure software implementations and is intended for consistent results between platforms and compilers. It is more feature rich than GameMath, but implements only a C++ interface, not a pure C one. It is also LGPL which limits the ability to static link a specific version of the library for projects that do not wish to license under the LGPL or follow its restrictions.
