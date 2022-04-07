QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    atmwindow.cpp \
    main.cpp \
    pageinsertcard.cpp \
    pagekeypad.cpp \
    pagemainaccountview.cpp \
    pagetransactions.cpp \
    pagewithdraw.cpp \
    userstatusbarwidget.cpp

HEADERS += \
    atmwindow.h \
    pageinsertcard.h \
    pagekeypad.h \
    pagemainaccountview.h \
    pagetransactions.h \
    pagewithdraw.h \
    userstatusbarwidget.h

FORMS += \
    atmwindow.ui \
    pageinsertcard.ui \
    pagekeypad.ui \
    pagemainaccountview.ui \
    pagetransactions.ui \
    pagewithdraw.ui \
    userstatusbarwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
