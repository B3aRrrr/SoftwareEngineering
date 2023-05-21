cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (WEB_SERVICE "web_service")
SET (PAGE_SERVICE "page_service")
SET (MESSAGE_SERVICE "message_service")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# EXAMPLE 

add_executable(${WEB_SERVICE} mains/web/web.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/user/user.cpp)


target_include_directories(${WEB_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${WEB_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${WEB_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${WEB_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${WEB_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)
#PAGE SERVICE

add_executable(${PAGE_SERVICE}   mains/page/page.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/page/page.cpp)


target_include_directories(${PAGE_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${PAGE_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${PAGE_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PAGE_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PAGE_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)

#MESSANGER SERVICE

add_executable(${MESSAGE_SERVICE} mains/message/message.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/message/message.cpp)


target_include_directories(${MESSAGE_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${MESSAGE_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${MESSAGE_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${MESSAGE_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${MESSAGE_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)






