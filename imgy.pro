TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    imgfile.cpp \
    imgeff.cpp \
    imgeff_warp.cpp \
    objs.cpp \
    render.cpp

HEADERS += \
    imgfile.hpp \
    imgeff.hpp \
    objs.hpp \
    glmath.hpp \
    render.hpp
