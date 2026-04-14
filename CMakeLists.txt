cmake_minimum_required(VERSION 3.19)
project(PopDict VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

qt_add_executable(PopDict ${SOURCES} ${HEADERS})

target_link_libraries(PopDict
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

install(TARGETS PopDict
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
