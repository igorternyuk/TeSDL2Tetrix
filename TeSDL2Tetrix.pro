TEMPLATE = app
#CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= DEBUG

SOURCES += main.cpp \
    well.cpp \
    controller.cpp \
    model.cpp \
    view.cpp \
    tetramino.cpp

LIBS += -lSDL2  -lSDL2_ttf -lSDL2_gfx

QMAKE_CXXFLAGS += -std=c++14

HEADERS += \
    well.h \
    controller.h \
    model.h \
    view.h \
    modellistener.h \
    tetramino.h
