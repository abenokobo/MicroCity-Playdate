# mac:
# No use cmake kit.
# cmake guess what compilers and env.

if (APPLE)
    set(__BUILD_MACHINE "Mac")

    if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
        # Generate Universal Binary only when building with M1.
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64 -arch arm64")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -arch arm64")
    endif()
else()
    set(__BUILD_MACHINE "Windows")

    # Use MinGW when on Windows.
    set(TOOLCHAIN_DIR $ENV{PLAYDATE_MINGW})
    set(TOOLCHAIN_PREFIX "x86_64-w64-mingw32-")
    set(TOOLCHAIN_POSTFIX ".exe")

    set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_POSTFIX})
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}g++${TOOLCHAIN_POSTFIX})
    set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
endif()
