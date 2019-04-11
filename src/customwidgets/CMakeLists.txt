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
        waterfall.hpp)

target_link_libraries(customwidgets
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::OpenGL)

target_include_directories(customwidgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testutility
        test/utilitytest.cpp
        test/utilitytest.hpp)

target_link_libraries(testutility
        Qt5::Core
        Qt5::Test
        customwidgets)

add_test(TestUtility
        testutility -o testresult-testutility.xml,xunitxml)

add_executable(testwaterfall
        test/waterfalltest.cpp
        test/waterfalltest.hpp)

target_link_libraries(testwaterfall
        Qt5::Core
        Qt5::Test
        customwidgets)

add_test(TestWaterfall
        testwaterfall -o testresult-testwaterfall.xml,xunitxml)
