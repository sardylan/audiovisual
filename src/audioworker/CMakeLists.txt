find_package(Qt5 REQUIRED COMPONENTS
        Core
        Multimedia
        Concurrent)

find_package(FFTW REQUIRED COMPONENTS
        FFTW_MT)

include_directories(${FFTW_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(audioworker
        worker.cpp
        worker.hpp)

target_link_libraries(audioworker
        fftutility
        dsp
        Qt5::Core
        Qt5::Multimedia
        Qt5::Concurrent
        FFTW::FFTW_MT
        fftw3)

target_include_directories(audioworker
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
