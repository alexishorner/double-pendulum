TEMPLATE = app
CONFIG += console c++14
QT += widgets


SOURCES = main.cpp \
          ../../../gui/src/doublependulum.cpp \
          ../../../gui/src/error.cpp \
          ../../../gui/src/globalconstants.cpp

HEADERS = ../../../gui/src/doublependulum.h \
          ../../../gui/src/error.h \
          ../../../gui/src/globalconstants.h



QMAKE_CXXFLAGS_DEBUG -= -O1
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O3
QMAKE_CXXFLAGS_DEBUG -= -Ofast
QMAKE_CXXFLAGS_DEBUG -= -Os
QMAKE_CXXFLAGS_DEBUG *= -O0

QMAKE_CXXFLAGS_RELEASE -= -O0
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE -= -Ofast
QMAKE_CXXFLAGS_RELEASE -= -Os
QMAKE_CXXFLAGS_RELEASE *= -O2

QMAKE_CXXFLAGS_RELEASE += -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal \
                          -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Wredundant-decls \
                          -Wdouble-promotion -Winit-self -Wundef -Wlogical-op -Wdisabled-optimization \
                          -Wunreachable-code -Wunsafe-loop-optimizations
