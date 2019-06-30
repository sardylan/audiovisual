find_package(Qt5 REQUIRED COMPONENTS
        Core
        Test)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(dsp
        dsp.cpp
        dsp.hpp)

target_link_libraries(dsp
        Qt5::Core
        fftutility
        fftw3)

target_include_directories(dsp
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testdsp
        test/dsptest.cpp
        test/dsptest.hpp)

target_link_libraries(testdsp
        Qt5::Core
        Qt5::Test
        dsp
        fftutility
        fftw3)

add_test(TestDSP
        testdsp -o testresult-testdsp.xml,xunitxml)
