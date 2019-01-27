TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/main.cpp \
    src/file/imgfile.cpp \
    src/render/objs.cpp \
    src/render/render.cpp \
    src/cmd/cmd.cpp \
    src/imgproc/imgproc.cpp \
    src/imgproc/warp.cpp \
    src/imgproc/ffd.cpp

HEADERS += \
    include/file/imgfile.hpp \
    include/render/objs.hpp \
    include/math/glmath.hpp \
    include/render/render.hpp \
    include/cmd/cmd.hpp \
    include/imgproc/imgproc.hpp \
    include/render/types.hpp
