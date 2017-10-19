include_directories(../include)

set(SOURCE_FILES_CLIENT source/main.c)
set(COMMON_HEADERS ../include/common_types.h)

add_executable(reservation_client ${SOURCE_FILES_CLIENT} ${COMMON_HEADERS})