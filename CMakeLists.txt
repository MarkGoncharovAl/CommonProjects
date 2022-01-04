cmake_minimum_required(VERSION 3.10)
project(Main VERSION 1.0.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3") 

find_package(SFML 2 COMPONENTS network graphics audio REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

add_subdirectory(Map)

message("Building!")

add_executable(
    run
    main.cpp  
)

target_link_libraries(
    run
    
    Map
    Hero

    sfml-graphics 
    sfml-network
    sfml-audio 
)