cmake_minimum_required(VERSION 2.8)

project(singularuty.modeling)

list(APPEND BOOST_COMPONENTS thread
                             date_time
                             filesystem
                             system
                             chrono
                             program_options
                             unit_test_framework
                             serialization
                             )

find_package(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})

if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory( libraries/singularity/ )

file(GLOB HEADERS "include/*.hpp")

set(SOURCE_EXE ${SOURCE_EXE} providers.cpp bitcoinexplorer.cpp)

if ( APPLE )
    set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -stdlib=libc++ -Wall -fpermissive" )
else ( APPLE )
    set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -fpermissive" )
endif ( APPLE )

include_directories( include )

add_executable( example programs/example.cpp ${HEADERS} ${SOURCE_EXE} )

target_link_libraries( example singularity boost_system bitcoin)

add_subdirectory( tests )

enable_testing ()
