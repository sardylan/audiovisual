find_package(Qt5 REQUIRED COMPONENTS
        Core
        Test)

find_package(FFTW REQUIRED COMPONENTS
        FFTW_MT)

include_directories(${FFTW_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(fftutility
        fft1d.cpp
        fft1d.hpp)

target_link_libraries(fftutility
        Qt5::Core
        fftw3)

target_include_directories(fftutility
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testftt1d
        test/fft1dtest.cpp
        test/fft1dtest.hpp)

target_link_libraries(testftt1d
        Qt5::Core
        Qt5::Test
        fftutility
        fftw3)

add_test(TestFFT1D
        testftt1d -o testresult-testftt1d.xml,xunitxml)
