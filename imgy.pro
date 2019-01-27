TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/main.cpp \
    src/render/render.cpp \
    src/cmd/cmd.cpp \
    src/imgproc/imgproc.cpp \
    src/imgproc/warp.cpp \
    src/imgproc/ffd.cpp \
    src/file/bmp.cpp \
    src/render/ray.cpp

HEADERS += \
    include/math/glmath.hpp \
    include/render/render.hpp \
    include/cmd/cmd.hpp \
    include/imgproc/imgproc.hpp \
    include/render/types.hpp \
    include/file/filebase.hpp \
    include/file/types.hpp \
    include/file/bmp.hpp \
    include/render/ray.hpp \
    include/render/objsbase.hpp \
    include/render/figures.hpp \
    include/render/light.hpp
