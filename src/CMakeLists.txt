file(GLOB LIB_SOURCES *.c)
file(GLOB LIB_HEADERS *.h)

add_executable(ducky ducky.c)

add_library(ducky_lib ${LIB_SOURCES} ${LIB_HEADERS})
target_link_libraries(ducky ducky_lib m)
