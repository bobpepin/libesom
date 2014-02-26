TEMPLATE = lib
CONFIG += debug_and_release build_all
CONFIG -= dll
CONFIG += static

TARGET = esom

HEADERS = esom.h

SOURCES = Grid.cc SOM.cc UMatrix.cc Watershed.cc

INCLUDEPATH += . ./esom
