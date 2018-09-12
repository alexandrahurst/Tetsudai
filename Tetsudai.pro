#-------------------------------------------------
#
# Project created by QtCreator 2016-09-09T14:40:45
#
#-------------------------------------------------

QT       += core gui svg xmlpatterns sql webenginewidgets

QMAKE_CXXFLAGS = -std=c++14 -lstdc++

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = Tetsudai
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    searchwidget.cpp \
    jmdict_dictionary.cpp \
    jmdict_sense.cpp \
    frequency_dictionary.cpp \
    termentry.cpp \
    searchengine.cpp \
    searchwidgeturlschemehandler.cpp \
    kanjidic2_dictionary.cpp \
    ocrengine.cpp \
    overlaywindow.cpp

HEADERS  += mainwindow.h \
    searchwidget.h \
    jmdict_entry.h \
    jmdict_dictionary.h \
    jmdict_kanjielement.h \
    jmdict_readingelement.h \
    jmdict_sense.h \
    jmdict_entity.h \
    frequency_dictionary.h \
    termentry.h \
    entry.h \
    scale.h \
    searchengine.h \
    searchwidgeturlschemehandler.h \
    kanjidic2_dictionary.h \
    kanjidic2_entry.h \
    ocrengine.h \
    overlaywindow.h

FORMS    += mainwindow.ui \
    searchwidget.ui \
    overlaywindow.ui

copydata.commands = $(COPY_DIR) $$PWD/res $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

DISTFILES +=

LIBS += /usr/lib/x86_64-linux-gnu/libtesseract.so.4
