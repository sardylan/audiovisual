add_subdirectory(customwidgets)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Svg
        Widgets
        OpenGL)

configure_file(version.hpp.in version.hpp @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(audiovisual
        windows/mainwindow.cpp
        windows/mainwindow.hpp
        windows/mainwindow.ui
        audiovisual.cpp
        audiovisual.hpp)

target_link_libraries(audiovisual
        customwidgets
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::OpenGL)
