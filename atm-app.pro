TEMPLATE = subdirs

SUBDIRS = atm-ui rfid pin-ui rest

atm-ui.subdir = atm-ui

rfid.file = atm-dll/rfid/AsyncSerialInterface.pro
pin-ui.file = atm-dll/pin-ui/Pin.pro
rest.file = atm-dll/rest/REST.pro

atm-ui.depends = rfid pin-ui rest
