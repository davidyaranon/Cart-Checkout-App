cmake_minimum_required(VERSION 3.7)

project(ucmercedcartapp1)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/include/libbson-1.0")
include_directories("/usr/local/lib")

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)

add_executable(ucmercedcartapp1 main.cpp)
target_include_directories(ucmercedcartapp1 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ucmercedcartapp1 ${Boost_LIBRARIES} Threads::Threads ${LIBMONGOCXX_LIBRARIES})
# target_link_libraries(ucmercedcartapp PRIVATE mongo::mongocxx_shared)
