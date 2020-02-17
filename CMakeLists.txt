cmake_minimum_required(VERSION 3.0.2)
project(cpman)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set ( SOURCES
    src/main.cpp    
    src/cpman.cpp
)

set ( HEADERS
    src/cpman.h
)

find_package( Qt5 REQUIRED COMPONENTS Widgets Core Gui)
add_executable( cpman ${SOURCES} )
target_link_libraries( cpman Qt5::Widgets Qt5::Core Qt5::Gui )
