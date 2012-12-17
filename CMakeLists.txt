cmake_minimum_required(VERSION 2.8)
project(lwqq)

include_directories(src/include/lwqq/)
include_directories(src/liblwqq/libghttp/)

add_definitions(-DUSE_CURL=1)
add_definitions(-DUSE_ZLIB=1)
add_definitions(-DUSE_SQLITE3=1)

SET(GHTTPSOURCES

	src/liblwqq/libghttp/ghttp.c
	src/liblwqq/libghttp/http_date.c
	src/liblwqq/libghttp/http_req.c
	src/liblwqq/libghttp/http_trans.c
	src/liblwqq/libghttp/http_base64.c
	src/liblwqq/libghttp/http_hdrs.c
	src/liblwqq/libghttp/http_resp.c
	src/liblwqq/libghttp/http_uri.c	
)

SET(LWQQSOURCES

	src/liblwqq/http.c
	src/liblwqq/info.c
	src/liblwqq/logger.c
	src/liblwqq/md5.c
	src/liblwqq/smemory.c
	src/liblwqq/unicode.c
	src/liblwqq/http_curl.c
	src/liblwqq/json.c
	src/liblwqq/login.c
	src/liblwqq/msg.c
	src/liblwqq/type.c
	src/liblwqq/url.c
)

add_library(liblwqq STATIC ${LWQQSOURCES} ${GHTTPSOURCES})

find_package(EV)
find_package(ZLIB)

if(EV_FOUND)
	target_link_libraries(liblwqq ${EV_LIBRARIES})
	include_directories(${EV_INCLUDE_DIR})
else()
	# USE BUNDLED ONE
	include_directories(${CMAKE_SOURCE_DIR}/libev)
	target_link_libraries(liblwqq libev)
endif()

if(ZLIB_FOUND)
	target_link_libraries(liblwqq ${ZLIB_LIBRARIES})
else()
	# USE BUNDLED ONE
	include_directories("${CMAKE_SOURCE_DIR}/zlib")
	target_link_libraries(liblwqq zlibstatic)
endif()

if(WIN32)
	# make sure we can find pthread.h
	include_directories("${CMAKE_SOURCE_DIR}/win32")
endif()
