cmake_minimum_required(VERSION 3.9)
project(artha VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB ARTHA_SOURCES
	"src/*.cpp"
	"src/*/*.cpp"
)

file(GLOB ARTHA_HEADER_FILES
	"src/*.h"
	"src/*/*.h"
)

set(ARTHA_INCLUDE_DIRS
	"src/"
	"3rdparty/"
	"3rdparty/termcolor/"
)

add_executable(artha ${ARTHA_SOURCES})

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
find_package(OpenSSL REQUIRED)
add_subdirectory(3rdparty/secp256k1/)

target_link_libraries(artha secp256k1 OpenSSL::SSL ${Boost_LIBRARIES})

target_include_directories(artha PUBLIC ${ARTHA_INCLUDE_DIRS})
