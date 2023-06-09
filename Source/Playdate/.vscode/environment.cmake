#
# SDK
#

string(SUBSTRING "${CMAKE_HOST_SYSTEM_NAME}" 0 4 _HOSTNAME)
if (_HOSTNAME STREQUAL "MSYS")
    set(CMAKE_HOST_WIN32 true)
endif()

if (APPLE)
    execute_process(
            COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
            COMMAND head -n 1
            COMMAND cut -c9-
            OUTPUT_VARIABLE SDK
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(LIB_EXTENSION dylib)
elseif (CMAKE_HOST_WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
    set(SDK $ENV{PLAYDATE_SDK_PATH})
    set(LIB_EXTENSION dll)
else ()
    message(FATAL_ERROR "Platform not supported!")
endif()


#
# DEFINE
#

add_definitions(-D_PLAYDATE)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()


#
# For Debug...
#

#message(STATUS "*** dump start cmake variables ***")
#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#        message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#message(STATUS "*** dump end ***")
