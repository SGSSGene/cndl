# SPDX-FileCopyrightText: 2020 Lutz Freitag
# SPDX-License-Identifier: CC0-1.0
cmake_minimum_required(VERSION 3.12)
project(cndl)
include(GNUInstallDirs)

find_package(PkgConfig)

include(cmake/CPM.cmake)
CPMAddPackage("gh:SGSSGene/cpm.dependencies@1.0.0")
CPMLoadDependenciesFile("${CMAKE_CURRENT_SOURCE_DIR}/cpm.dependencies")

file(GLOB_RECURSE CPP_SRCS src/cndl/*.cpp)

add_library(cndl SHARED ${CPP_SRCS})
add_library(cndl::cndl ALIAS cndl)

target_include_directories(cndl
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(cndl PUBLIC cxx_std_20)


pkg_check_modules(CRYPTO REQUIRED libcrypto)
target_include_directories(cndl PUBLIC ${CRYPTO_INCLUDE_DIRS})
target_compile_options(cndl PRIVATE ${CRYPTO_CFLAGS})


target_link_libraries(cndl
    ${CRYPTO_LIBRARIES}
    simplyfile::simplyfile
)

execute_process(
        COMMAND git describe --tags
        OUTPUT_VARIABLE VERSION
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        ERROR_VARIABLE VERSION_ERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE)

if(VERSION STREQUAL "")
    message(FATAL_ERROR "cannot generate version string")
endif()

message("cndl version: " ${VERSION})

configure_file(cndl.pc.in cndl.pc @ONLY)

install(TARGETS cndl DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES
    src/cndl/base64.h
    src/cndl/ConnectionHandler.h
    src/cndl/Dispatcher.h
    src/cndl/Error.h
    src/cndl/Extractor.h
    src/cndl/ProtocolHandler.h
    src/cndl/Request.h
    src/cndl/Response.h
    src/cndl/Route.h
    src/cndl/Server.h
    src/cndl/StaticFileHandler.h
    src/cndl/unique_function.h
    src/cndl/Websocket.h
    src/cndl/WSRoute.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cndl)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cndl.pc
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pkgconfig)
install(FILES LICENSE
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/licenses/${PROJECT_NAME})
