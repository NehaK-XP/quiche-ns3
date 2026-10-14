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
    TEST_SOURCES test/ns3-quiche-test-suite.cc
                 ${examples_as_tests_sources}
)
