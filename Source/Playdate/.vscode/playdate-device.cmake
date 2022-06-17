# For add link option, windows sdk path
# Toolchain is equal
#   PlaydateSDK/C_API/buildsupport/arm.cmake
#
# [[Modified for Win32]]
# - Add postfix to exec file.
# - define __BUILD_MACHINE

if (APPLE)
   
else()
    set(SDK $ENV{PLAYDATE_SDK_PATH})
endif()

add_link_options(-specs=nano.specs -specs=nosys.specs)



#
# Toolchain
#

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(TOOLCHAIN_PREFIX arm-none-eabi-)

if (APPLE)
    set(__BUILD_MACHINE "Mac")
    set(TOOLCHAIN_DIR "/usr/local/playdate/gcc-arm-none-eabi-9-2019-q4-major")
else()
    set(__BUILD_MACHINE "Windows")
    set(TOOLCHAIN_DIR $ENV{PLAYDATE_ARM_GCC})
    set(TOOLCHAIN_POSTFIX .exe)
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_POSTFIX})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}g++${TOOLCHAIN_POSTFIX})
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

set(CMAKE_OBJCOPY ${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}objcopy${TOOLCHAIN_POSTFIX} CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}size${TOOLCHAIN_POSTFIX} CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR}/bin)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(TOOLCHAIN armgcc)

MESSAGE(STATUS "playdate.cmake loaded")
