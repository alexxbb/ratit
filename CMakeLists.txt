cmake_minimum_required(VERSION 3.13)
project(ratit)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH /opt/Qt/5.11.2/gcc_64/lib/cmake/Qt5;/opt/hfs17.5/toolkit/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)
find_package( Houdini REQUIRED )

include_directories(.)


file(GLOB SOURCES "src/*.cpp")

qt5_add_resources(RESOURCES resources/resources.qrc)

add_executable(ratit
        ${SOURCES}
        ${RESOURCES}
        )

target_link_libraries(ratit Qt5::Widgets Qt5::Core Qt5::Gui Houdini)
