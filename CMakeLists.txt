cmake_minimum_required(VERSION 2.8)
project(lwqq)

include_directories(src/include/lwqq/)
include_directories(src/liblwqq/libghttp/)

add_definitions(-DUSE_CURL=1)
add_definitions(-DUSE_ZLIB=1)
add_definitions(-DUSE_SQLITE3=1)

SET(LWQQSOURCES

	src/liblwqq/info.c
	src/liblwqq/logger.c
	src/liblwqq/md5.c
	src/liblwqq/smemory.c
	src/liblwqq/unicode.c
	src/liblwqq/http_curl.cpp
	src/liblwqq/async.cpp
	src/liblwqq/json.c
	src/liblwqq/login.c
	src/liblwqq/msg.cpp
	src/liblwqq/type.c
	src/liblwqq/url.c
)

add_library(liblwqq STATIC ${LWQQSOURCES})

find_package(EV REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

include_directories(${EV_INCLUDE_DIR})
target_link_libraries(liblwqq ${EV_LIBRARIES})
target_link_libraries(liblwqq ${ZLIB_LIBRARIES})
target_link_libraries(liblwqq ${CURL_LIBRARIES})

if(WIN32)
	# make sure we can find pthread.h
	include_directories("${CMAKE_SOURCE_DIR}/win32")
endif()