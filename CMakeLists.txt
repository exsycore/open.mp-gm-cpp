cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

project(omp-server VERSION 0.1.0 LANGUAGES C CXX)

if(MSVC)
    foreach(flag_var
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE)
        if(${flag_var} MATCHES "/MT")
            string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

set(CONNECTOR_CPP_INCLUDE "D:/open.mp/omp-nx-cpp/lib/mysql")
set(CONNECTOR_CPP_LIB "D:/open.mp/omp-nx-cpp/lib/mysql/lib/vs14")

include_directories(
    ${CONNECTOR_CPP_INCLUDE}
    ${PROJECT_SOURCE_DIR}/lib/encryption
)

link_directories(${CONNECTOR_CPP_LIB})

foreach(OUTPUTCONFIG DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "D:/open.mp/nx-server-omp/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "D:/open.mp/nx-server-omp/")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "D:/open.mp/nx-server-omp/")
endforeach()

add_subdirectory(lib/sdk)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/main.h

    # UTILITY
    src/utils/utils.cpp
    src/utils/utils.h

    # MYSQL SYSTEM
    src/mysql/mysql.cpp
    src/mysql/mysql.h

    # DIALOGS SYSTEM
    src/dialogs/dialogs.h
)

add_definitions(-DSTATIC_CONCPP)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OMP-SDK
    mysqlcppconn-static
    libssl
    libcrypto
)

target_sources(${PROJECT_NAME} PRIVATE
    lib/encryption/encrypt.cpp
    lib/encryption/Base64.cpp
    lib/encryption/vigenere.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY PREFIX "")

if(TARGET_BUILD_ARCH MATCHES "(i[3-6]86|x86)" AND NOT TARGET_BUILD_ARCH MATCHES "(x64|x86_64|amd64)")
    set(CMAKE_C_FLAGS "-m32 ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-m32 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_SIZEOF_VOID_P 4)
endif ()
