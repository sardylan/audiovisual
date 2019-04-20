find_package(Qt5 REQUIRED COMPONENTS
        Core
        Multimedia)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(audioworker
        worker.cpp
        worker.hpp)

target_link_libraries(audioworker
        Qt5::Core
        Qt5::Multimedia)

target_include_directories(audioworker
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
