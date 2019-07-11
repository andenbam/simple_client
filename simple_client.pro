QT += core gui widgets network
CONFIG += app_bundle
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        myclient.cpp \
        testexternaladdress.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    myclient.h \
    testexternaladdress.h

DISTFILES +=
