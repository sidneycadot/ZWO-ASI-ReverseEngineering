
CONFIG += debug
TEMPLATE = app

QMAKE_CXXFLAGS += -W -Wall -O3 -std=c++11 -ffast-math

LIBS += libASICamera2.a -lusb-1.0

TARGET = AsiViewer

SOURCES += main.cpp

HEADERS += Application.h
SOURCES += Application.cpp

HEADERS += MainWindow.h
SOURCES += MainWindow.cpp

HEADERS += CentralWidget.h
SOURCES += CentralWidget.cpp

HEADERS += AsiThread.h
SOURCES += AsiThread.cpp
