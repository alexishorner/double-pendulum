CONFIG += c++14
CONFIG += warn_on
QT += widgets
QT += charts

SOURCES = doublependulum.cpp \
          doubleslider.cpp \
          globalconstants.cpp \
          main.cpp \
          mainwindow.cpp \
          drawingarea.cpp \
          grapharea.cpp \
          error.cpp \
          doublespinbox.cpp

HEADERS = doublependulum.h \
          doubleslider.h \
          globalconstants.h \
          mainwindow.h \
          drawingarea.h \
          grapharea.h \
          error.h \
          doublespinbox.h

TRANSLATIONS = simulation_fr.ts
CODECFORSRC = UTF-8

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
