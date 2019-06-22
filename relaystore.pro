######################################################################
# Automatically generated by qmake (2.01a) ?? ???? 27 08:32:42 2011
######################################################################

TEMPLATE = app
TARGET = relaystore
DEPENDPATH += . src
INCLUDEPATH += . src

#QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++

# Input
HEADERS += src/rs_about.h \
           src/rs_add_dialog.h \
           src/rs_auth_dialog.h \
           src/rs_change_dialog.h \
           src/rs_history.h \
           src/rs_html.h \
           src/rs_main.h \
           src/rs_main_table.h \
           src/rs_query_dialog.h \
           src/rs_settings.h \
    src/rs_manager.h \
    src/rs_settings_manager.h \
    src/RelayTableModel.h \
    src/servers.h \
    src/types.h
SOURCES += main.cpp \
           src/rs_about.cpp \
           src/rs_add_dialog.cpp \
           src/rs_auth_dialog.cpp \
           src/rs_change_dialog.cpp \
           src/rs_history.cpp \
           src/rs_html.cpp \
           src/rs_main.cpp \
           src/rs_main_table.cpp \
           src/rs_query_dialog.cpp \
           src/rs_settings.cpp \
    src/rs_manager.cpp \
    src/rs_settings_manager.cpp \
    src/RelayTableModel.cpp \
    src/servers.cpp

RESOURCES += \
    res.qrc

QT += sql core sql gui widgets

DISTFILES += \
    settings.ini.sample
