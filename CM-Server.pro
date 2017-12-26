TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tcpserver.cpp \
    cmserver.cpp

HEADERS += \
    commen.h \
    tcpserver.h \
    cmserver.h \
    sharedata.h

LIBS += -lpthread
