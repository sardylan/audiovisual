find_package(OpenGL REQUIRED COMPONENTS
        OpenGL
        GLX)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Svg
        Widgets
        OpenGL
        Test)

add_library(customwidgets
        vumeter.cpp
        vumeter.hpp
        utility.cpp
        utility.hpp
        waterfall.cpp
        waterfall.hpp
        waterfall_color_table.hpp)

target_link_libraries(customwidgets
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::OpenGL
        OpenGL::OpenGL
        OpenGL::GLU)

target_include_directories(customwidgets
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testutility
        test/utilitytest.cpp
        test/utilitytest.hpp)

target_link_libraries(testutility
        Qt5::Core
        Qt5::Test
        customwidgets)

add_test(TestUtility
        testutility -o testresult-testutility.xml,xunitxml)
