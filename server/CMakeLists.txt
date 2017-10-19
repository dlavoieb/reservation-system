include_directories(../include)

set(SOURCE_FILES_SERVER source/main.c)
set(COMMON_HEADERS ../include/common_types.h ../include/names.h)

add_executable(reservation_server ${SOURCE_FILES_SERVER} ${COMMON_HEADERS})
target_link_libraries(reservation_server rt pthread)