project(so)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    cmake_minimum_required(VERSION 3.0.0)
else()
    cmake_minimum_required(VERSION 2.8.0)
endif()

set(CMAKE_CXX_FLAGS "-g -std=c++11 -Wall -W -Werror -fPIC")

set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/sw/so)

# nlohmann/json.hpp dependency
find_path(JSON_HEADER nlohmann)

# libtinyxml2 dependency
find_library(TINYXML2_LIB libtinyxml2.a)

# libarchive dependency
find_library(ARCHIVE_LIB libarchive.a)

# preparation::posts_parser
set(POSTS_PARSER posts_parser)
set(POSTS_PARSER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/preparation)

add_executable(${POSTS_PARSER} ${POSTS_PARSER_SOURCE_DIR}/posts_parser.cpp ${POSTS_PARSER_SOURCE_DIR}/seven_zip_reader.cpp ${POSTS_PARSER_SOURCE_DIR}/record_reader.cpp)

target_include_directories(${POSTS_PARSER} PUBLIC ${JSON_HEADER})
target_include_directories(${POSTS_PARSER} PUBLIC ${TINYXML2_HEADER})
target_link_libraries(${POSTS_PARSER} lzma z bz2)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(${POSTS_PARSER} iconv)
endif()

target_link_libraries(${POSTS_PARSER} ${TINYXML2_LIB})
target_link_libraries(${POSTS_PARSER} ${ARCHIVE_LIB})

# preparation::users_parser
set(USERS_PARSER users_parser)
set(USERS_PARSER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/preparation)

add_executable(${USERS_PARSER} ${USERS_PARSER_SOURCE_DIR}/users_parser.cpp ${USERS_PARSER_SOURCE_DIR}/seven_zip_reader.cpp ${USERS_PARSER_SOURCE_DIR}/record_reader.cpp)

target_include_directories(${USERS_PARSER} PUBLIC ${JSON_HEADER})
target_include_directories(${USERS_PARSER} PUBLIC ${TINYXML2_HEADER})
target_link_libraries(${USERS_PARSER} lzma z bz2)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(${USERS_PARSER} iconv)
endif()

target_link_libraries(${USERS_PARSER} ${TINYXML2_LIB})
target_link_libraries(${USERS_PARSER} ${ARCHIVE_LIB})
