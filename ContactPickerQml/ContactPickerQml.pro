APP_NAME = ContactPickerQml

CONFIG += qt warn_on cascades10

INCLUDEPATH += ../src ${QNX_TARGET}/usr/include/bb/cascades/picker\
DEPENDPATH += ../src ${QNX_TARGET}/usr/include/bb/cascades/pickers
LIBS += -lbbcascadespickers -lbbpim

include(config.pri)
