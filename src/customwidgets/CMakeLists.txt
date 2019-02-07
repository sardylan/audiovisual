find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Svg
        Widgets
        OpenGL)

add_library(customwidgets
        vumeter.cpp
        vumeter.hpp)

target_link_libraries(customwidgets
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::OpenGL)

target_include_directories(customwidgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})