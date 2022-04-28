QT += network
QT += gui


TEMPLATE = lib
DEFINES += REST_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    banktransaction.cpp \
    restbalancedata.cpp \
    restengine.cpp \
    restinfodata.cpp \
    restinterface.cpp \
    restreturndata.cpp \
    resttransactiondata.cpp

HEADERS += \
    REST_global.h \
    banktransaction.h \
    restbalancedata.h \
    restengine.h \
    resterrorcode.h \
    restinfodata.h \
    restinterface.h \
    restreturndata.h \
    restreturnheader.h \
    resttransactiondata.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
