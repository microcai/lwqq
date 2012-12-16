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

target_link_libraries(liblwqq -lev)
target_link_libraries(liblwqq -lz)
target_link_libraries(liblwqq -lpthread)