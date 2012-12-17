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
	src/liblwqq/http_curl.c
	src/liblwqq/async.c
	src/liblwqq/json.c
	src/liblwqq/login.c
	src/liblwqq/msg.c
	src/liblwqq/type.c
	src/liblwqq/url.c
)

add_library(liblwqq STATIC ${LWQQSOURCES})

find_package(EVENT)
find_package(ZLIB)
find_package(CURL)

if(EVENT_FOUND)
	target_link_libraries(liblwqq ${EVENT_LIBRARIES})
	include_directories(${EVENT_INCLUDE_DIR})
else()
	# USE BUNDLED ONE
	include_directories(${CMAKE_SOURCE_DIR}/libevent/include)
	target_link_libraries(liblwqq libevent)
	if(WIN32)
		include_directories("${CMAKE_SOURCE_DIR}/libevent/WIN32-Code")
	endif()
endif()

if(ZLIB_FOUND)
	target_link_libraries(liblwqq ${ZLIB_LIBRARIES})
else()
	# USE BUNDLED ONE
	include_directories("${CMAKE_SOURCE_DIR}/zlib")
	target_link_libraries(liblwqq zlibstatic)
endif()

if(CURL_FOUND)
	target_link_libraries(liblwqq ${CURL_LIBRARIES})
endif()

if(WIN32)
	# make sure we can find pthread.h
	include_directories("${CMAKE_SOURCE_DIR}/win32")
endif()
