find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Svg
        Widgets
        OpenGL
        Multimedia)

configure_file(version.hpp.in version.hpp @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(audiovisual
        windows/mainwindow.cpp
        windows/mainwindow.hpp
        windows/mainwindow.ui
        windows/configwindow.cpp
        windows/configwindow.hpp
        windows/configwindow.ui
        audiovisual.cpp
        audiovisual.hpp)

target_link_libraries(audiovisual
        customwidgets
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::OpenGL
        Qt5::Multimedia)
