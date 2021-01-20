TEMPLATE = app TARGET = HoloNotify
RC_ICONS = HoloNotify.ico
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets KNotifications network
SOURCES += main.cpp window.cpp tubers.cpp
HEADERS += window.h tubers.h \
    rapidjson/document.h \

