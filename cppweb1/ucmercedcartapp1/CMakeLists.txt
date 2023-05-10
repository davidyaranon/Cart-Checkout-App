cmake_minimum_required(VERSION 3.14)

project(ucmercedcartapp1)

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
find_package(OpenSSL REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/include/libbson-1.0")
# include_directories("/usr/local/lib")
# include_directories("/usr/local/lib/boost")
# include_directories("/usr/local/include")
# include_directories("/usr/local/include/crow")
# include_directories("/usr/include")
# include_directories("/usr/include/asio")
# include_directories("/usr/include/crow")
# include_directories("/usr/lib/")
# include_directories("/usr/include/boost")
# include_directories("/usr/lib/cmake/Crow")
# include_directories("/usr/share/doc/libasio-dev")
# set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/include/boost")
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")

# set(BOOST_ROOT /usr/local/)
# set(BOOST_LIBRARYDIR /usr/local/lib/)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)

# find_package(Crow)

add_executable(ucmercedcartapp1 main.cpp)
target_include_directories(ucmercedcartapp1 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ucmercedcartapp1 PRIVATE ${Boost_LIBRARIES} Threads::Threads ${LIBMONGOCXX_LIBRARIES} /usr/local/lib/libbcrypt.a OpenSSL::Crypto PUBLIC) # Crow::Crow)

# target_link_libraries(ucmercedcartapp1 ${Boost_LIBRARIES} Threads::Threads ${LIBMONGOCXX_LIBRARIES})
# target_link_libraries(ucmercedcartapp1 /usr/local/lib/libbcrypt.a)
# target_link_libraries(ucmercedcartapp1 OpenSSL::Crypto)
# target_link_libraries(ucmercedcartapp1 PUBLIC Crow::Crow)

set(CROW_FEATURES "ssl;compression")
# target_link_libraries(ucmercedcartapp PRIVATE mongo::mongocxx_shared)
