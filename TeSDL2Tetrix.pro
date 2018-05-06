TEMPLATE = app
CONFIG += c++1z
CONFIG -= app_bundle
CONFIG -= qt
DEFINES -= DEBUG
LIBS += -lSDL2  -lSDL2_ttf -lSDL2_image -lSDL2_mixer

SOURCES += main.cpp \
    well.cpp \
    controller.cpp \
    model.cpp \
    view.cpp \
    tetramino.cpp \
    sdl2initializer.cpp



HEADERS += \
    sdl2initializer.hpp \
    controller.hpp \
    model.hpp \
    modellistener.hpp \
    tetramino.hpp \
    view.hpp \
    well.hpp
