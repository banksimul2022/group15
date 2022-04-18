QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    atmwindow.cpp \
    main.cpp \
    pageaccountinfo.cpp \
    pagebase.cpp \
    pageinsertcard.cpp \
    pagekeypad.cpp \
    pageloading.cpp \
    pagemainaccountview.cpp \
    pagewithdraw.cpp \
    pagewithuserbar.cpp \
    userstatusbarwidget.cpp \
    utility.cpp

HEADERS += \
    atmwindow.h \
    pageaccountinfo.h \
    pagebase.h \
    pageinsertcard.h \
    pagekeypad.h \
    pageloading.h \
    pagemainaccountview.h \
    pagewithdraw.h \
    pagewithuserbar.h \
    statemanager.h \
    userstatusbarwidget.h \
    utility.h

FORMS += \
    atmwindow.ui \
    pageaccountinfo.ui \
    pageinsertcard.ui \
    pagekeypad.ui \
    pageloading.ui \
    pagemainaccountview.ui \
    pagewithdraw.ui \
    userstatusbarwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
