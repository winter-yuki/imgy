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
    src/render/figures.cpp \
    src/render/light.cpp \
    src/imgproc/luteffects.cpp \
    src/imgproc/bayer.cpp \
    src/imgproc/errdiff.cpp \

HEADERS += \
    include/math/glmath.hpp \
    include/cmd/cmd.hpp \
    include/file/filebase.hpp \
    include/file/bmp.hpp \
    include/render/render.hpp \
    include/render/types.hpp \
    include/render/ray.hpp \
    include/render/objsbase.hpp \
    include/render/figures.hpp \
    include/render/light.hpp \
    include/imgproc/imgproc.hpp \
    include/imgproc/luteffects.hpp \
    include/imgproc/bayer.hpp \
    include/imgproc/errdiff.hpp \
    include/imgproc/warp.hpp \
    include/imgproc/ffd.hpp \
    include/imgproc/types.hpp \
    include/utility/color.hpp \
