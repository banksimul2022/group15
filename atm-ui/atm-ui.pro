QT       += core gui svg serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/atmwindow.cpp \
    src/main.cpp \
    src/page/pageaccountinfo.cpp \
    src/page/abstract/pagebase.cpp \
    src/page/pagebufferguide.cpp \
    src/page/pageinsertcard.cpp \
    src/page/pagekeypad.cpp \
    src/page/pageloading.cpp \
    src/page/pagemainaccountview.cpp \
    src/page/pageprompt.cpp \
    src/page/pagewithdraw.cpp \
    src/page/abstract/pagewithuserbar.cpp \
    src/pagereturn.cpp \
    src/transactiontablemodel.cpp \
    src/userstatusbarwidget.cpp \
    src/utility.cpp

HEADERS += \
    include/atmwindow.h \
    include/page/pageaccountinfo.h \
    include/page/abstract/pagebase.h \
    include/page/pagebufferguide.h \
    include/page/pageinsertcard.h \
    include/page/pagekeypad.h \
    include/page/pageloading.h \
    include/page/pagemainaccountview.h \
    include/page/pageprompt.h \
    include/page/pageprompt_enum.h \
    include/page/pagewithdraw.h \
    include/page/abstract/pagewithuserbar.h \
    include/pagemanager.h \
    include/pagereturn.h \
    include/userstatusbarwidget.h \
    include/utility.h \
    include/transactiontablemodel.h

INCLUDEPATH += include

FORMS += \
    form/atmwindow.ui \
    form/page/pageaccountinfo.ui \
    form/page/pagebufferguide.ui \
    form/page/pageinsertcard.ui \
    form/page/pagekeypad.ui \
    form/page/pageloading.ui \
    form/page/pagemainaccountview.ui \
    form/page/pageprompt.ui \
    form/page/pagewithdraw.ui \
    form/userstatusbarwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

equals(STANDALONE, 1) {
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../atm-dll/rfid/build/release/ -lAsyncSerialInterface
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../atm-dll/rfid/build/debug/ -lAsyncSerialInterface
    else:unix: LIBS += -L$$PWD/../atm-dll/rfid/build/ -lAsyncSerialInterface

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../atm-dll/rest/build/release/ -lREST
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../atm-dll/rest/build/debug/ -lREST
    else:unix: LIBS += -L$$PWD/../atm-dll/rest/build/ -lREST

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../atm-dll/pin-ui/build/release/ -lPin
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../atm-dll/pin-ui/build/debug/ -lPin
    else:unix: LIBS += -L$$PWD/../atm-dll/pin-ui/build/ -lPin
} else {
    unix|win32: LIBS += -L$$PWD/../build/atm-dll/rfid/ -lAsyncSerialInterface
    unix|win32: LIBS += -L$$PWD/../build/atm-dll/rest/ -lREST
    unix|win32: LIBS += -L$$PWD/../build/atm-dll/pin-ui/ -lPin
}

INCLUDEPATH += $$PWD/../atm-dll/rfid
DEPENDPATH += $$PWD/../atm-dll/rfid

INCLUDEPATH += $$PWD/../atm-dll/rest
DEPENDPATH += $$PWD/../atm-dll/rest

INCLUDEPATH += $$PWD/../atm-dll/pin-ui
DEPENDPATH += $$PWD/../atm-dll/pin-ui
