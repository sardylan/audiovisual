find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Svg
        Widgets
        SerialPort
        OpenGL)

configure_file(version.hpp.in version.hpp @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(audiovisual
        audiovisual.cpp
        audiovisual.hpp)

target_link_libraries(audiovisual
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::OpenGL)
