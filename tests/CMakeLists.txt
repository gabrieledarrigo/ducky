FILE(GLOB TESTS *.c)

FOREACH(test ${TESTS})
    GET_FILENAME_COMPONENT(filename ${test} NAME_WE)
    add_executable(${filename} ${filename}.c)
    target_link_libraries(${filename} ${CMAKE_SOURCE_DIR}/lib/libtap.so ducky_lib)
    add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})
ENDFOREACH()