set(QUICHE_PREFIX "" CACHE PATH "Path to quiche source/build tree")

if("${QUICHE_PREFIX}" STREQUAL "")
   set(QUICHE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/third_party/quiche" CACHE PATH "Path to quiche source/build tree" FORCE)
endif()

set(QUICHE_INCLUDE_DIRS
    ${QUICHE_PREFIX}/quiche/include
)

set(QUICHE_LIB_SEARCH_PATHS
    ${QUICHE_PREFIX}/target/debug
    ${QUICHE_PREFIX}/target
)

find_library(QUICHE_LIB
    NAMES libquiche.a
    PATHS ${QUICHE_LIB_SEARCH_PATHS}
    NO_DEFAULT_PATH
)

find_path(QUICHE_HEADER_DIR
    NAMES quiche.h
    PATHS ${QUICHE_INCLUDE_DIRS}
    NO_DEFAULT_PATH
)

if(NOT QUICHE_LIB OR NOT QUICHE_HEADER_DIR)
   message(WARNING "[quiche-ns3] libquiche.a or quiche.h not found under "
                   "${QUICHE_PREFIX}. Build quiche with "
                   "'cargo build --release --feature ffi' or set "
                   "-DQUICHE_PREFIX=/path/to/quiche. SKipping quichem module.")
    return()
endif()

find_package(Threads REQUIRED)

set(_quiche_link_libs Threads::Threads ${CMAKE_DL_LIBS} m)
if(UNIX)
  list(APPEND _quiche_link_libs stdc++ gcc_s)
endif()


message(STATUS "[quiche-ns3] QUICHE_PREFIX = ${QUICHE_PREFIX}")
message(STATUS "[quiche-ns3] QUICHE_LIB = ${QUICHE_LIB}")
message(STATUS "[quiche-ns3] QUICHE_HEADER_DIR = ${QUICHE_HEADER_DIR}")

add_library(quiche::quiche STATIC IMPORTED)
set_target_properties(quiche::quiche PROPERTIES
     IMPORTED_LOCATION "${QUICHE_LIB}"
     INTERFACE_INCLUDE_DIRECTORIES "${QUICHE_HEADER_DIR}"
     INTERFACE_LINK_LIBRARIES "${_quiche_link_libs}")


check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/ns3-quiche-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME ns3-quiche
    SOURCE_FILES model/ns3-quiche.cc
                 helper/ns3-quiche-helper.cc
    HEADER_FILES model/ns3-quiche.h
                 helper/ns3-quiche-helper.h
    LIBRARIES_TO_LINK ${libcore}
                      ${libnetwork}
                      ${libinternet}
                      quiche::quiche
    TEST_SOURCES test/ns3-quiche-test-suite.cc
                 ${examples_as_tests_sources}
)
