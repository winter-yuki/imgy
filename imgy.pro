TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/main.cpp \
    src/file/imgfile.cpp \
    src/imgproc/imgeff.cpp \
    src/imgproc/imgeff_warp.cpp \
    src/render/objs.cpp \
    src/render/render.cpp \
    src/cmd/cmd.cpp

HEADERS += \
    include/file/imgfile.hpp \
    include/imgproc/imgeff.hpp \
    include/render/objs.hpp \
    include/math/glmath.hpp \
    include/render/render.hpp \
    include/cmd/cmd.hpp
