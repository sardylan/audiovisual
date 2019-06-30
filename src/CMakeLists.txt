find_library(FFTW
        fftw3)

include_directories($ENV{FFTW_DIR}/include)
link_directories($ENV{FFTW_DIR}/bin)

find_library(FREEGLUT
        freeglut)

include_directories($ENV{FREEGLUT_DIR}/include)
link_directories($ENV{FREEGLUT_DIR}/bin)

add_subdirectory(fftutility)
add_subdirectory(dsp)
add_subdirectory(customwidgets)
add_subdirectory(audioworker)
add_subdirectory(audiovisual)
