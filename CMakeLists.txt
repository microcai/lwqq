cmake_minimum_required(VERSION 2.8)
project(lwqq)

include_directories(src/include/lwqq/)
include_directories(src/liblwqq/libghttp/)

add_library(liblwqq STATIC
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