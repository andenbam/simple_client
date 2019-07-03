QT += core gui widgets network
CONFIG += app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        clientwindow.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    clientwindow.ui

HEADERS += \
    clientwindow.h
