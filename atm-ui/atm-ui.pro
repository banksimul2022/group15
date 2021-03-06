QT       += core gui svg serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/atmwindow.cpp \
    src/atmwindow_manager_impl.cpp \
    src/main.cpp \
    src/page/pageaccountinfo.cpp \
    src/page/abstract/pagebase.cpp \
    src/page/pagebufferguide.cpp \
    src/page/pageinsertcard.cpp \
    src/page/dialog/pagekeypad.cpp \
    src/page/pageloading.cpp \
    src/page/pagemainaccountview.cpp \
    src/page/dialog/pageprompt.cpp \
    src/page/pagewithdraw.cpp \
    src/page/abstract/pagewithuserbar.cpp \
    src/page/util/pagereturn.cpp \
    src/page/util/pagewidgetwrapper.cpp \
    src/page/util/transactiontablemodel.cpp \
    src/page/util/userstatusbarwidget.cpp \
    src/utility.cpp

HEADERS += \
    include/atmwindow.h \
    include/page/pageaccountinfo.h \
    include/page/abstract/pagebase.h \
    include/page/pagebufferguide.h \
    include/page/pageinsertcard.h \
    include/page/dialog/pagekeypad.h \
    include/page/pageloading.h \
    include/page/pagemainaccountview.h \
    include/page/dialog/pageprompt.h \
    include/page/dialog/pageprompt_enum.h \
    include/page/pagewithdraw.h \
    include/page/abstract/pagewithuserbar.h \
    include/page/util/pagewidgetwrapper.h \
    include/pagemanager.h \
    include/page/util/pagereturn.h \
    include/page/util/userstatusbarwidget.h \
    include/utility.h \
    include/page/util/transactiontablemodel.h

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

TRANSLATIONS = lang/atm-ui_en-GB.ts

RC_ICONS = files/icons/icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

equals(STANDALONE, 1): MODULE_BASE_PATH = ../atm-dll/_MODULE_/build/
else: MODULE_BASE_PATH = ../build/atm-dll/_MODULE_

defineReplace(modulePath) {
  return($$PWD/$$replace(MODULE_BASE_PATH, _MODULE_, $$1))
}

win32:CONFIG(release, debug|release): LIBS += -L$$modulePath(rfid)/release/ -lAsyncSerialInterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$modulePath(rfid)/debug/ -lAsyncSerialInterface
else:unix: LIBS += -L$$modulePath(rfid) -lAsyncSerialInterface

win32:CONFIG(release, debug|release): LIBS += -L$$modulePath(rest)/release/ -lREST
else:win32:CONFIG(debug, debug|release): LIBS += -L$$modulePath(rest)/debug/ -lREST
else:unix: LIBS += -L$$modulePath(rest) -lREST

win32:CONFIG(release, debug|release): LIBS += -L$$modulePath(pin-ui)/release/ -lPin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$modulePath(pin-ui)/debug/ -lPin
else:unix: LIBS += -L$$modulePath(pin-ui) -lPin

INCLUDEPATH += $$PWD/../atm-dll/rfid
DEPENDPATH += $$PWD/../atm-dll/rfid

INCLUDEPATH += $$PWD/../atm-dll/rest
DEPENDPATH += $$PWD/../atm-dll/rest

INCLUDEPATH += $$PWD/../atm-dll/pin-ui
DEPENDPATH += $$PWD/../atm-dll/pin-ui
