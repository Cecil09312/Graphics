#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T10:35:22
#
#-------------------------------------------------

QT       += core gui quick svg  concurrent sql texttospeech serialport  printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicsDisplay
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

 CONFIG -= debug_and_release
 OBJECTS_DIR =./obj
 MOC_DIR =./moc
 RCC_DIR = ./rcc
 DESTDIR = ./bin

SOURCES += \
        main.cpp \
        crtwidget.cpp \
    architePlan/architeplanview.cpp \
    infoTable/infotableview.cpp \
    graphicsWidget/graphicsscene.cpp \
    graphicsWidget/graphicsview.cpp \
    graphicsWidget/graphicsitem.cpp \
    architePlan/treeview.cpp \
    jsonEdit/jsonedit.cpp \
    control/controller.cpp \
    dataStore/datastore.cpp \
    speech/speechobj.cpp \
    database/sqlitemanager.cpp \
    database/sqlmanager.cpp \
    architePlan/sysarchiteplanview.cpp \
    qmlTableModel/qmltablemodel.cpp \
    jsonEdit/qmlforjson.cpp \
    control/usermanager.cpp \
    jsonEdit/itemiconinfotojson.cpp \
    architePlan/styleditemdelegate.cpp \
    communication/abstractconfiguration.cpp \
    communication/abstractlink.cpp \
    communication/seriallink.cpp \
    communication/tcplink.cpp \
    communication/serialconfiguration.cpp \
    communication/tcpconfiguration.cpp \
    communication/configurationmanager.cpp \
    communication/udplink.cpp \
    communication/udpconfiguration.cpp \
    communication/canconfiguration.cpp \
    database/mysqlmanager.cpp \
    dataStore/abstractdataprotocol.cpp \
    dataStore/serialdataprotocol.cpp \
    dataStore/indicatordataprotocol.cpp \
    dataStore/monitoringprotocol.cpp \
    graphicsWidget/globalgraphicsview.cpp \
    graphicsWidget/globalgraphicsscene.cpp \
    graphicsWidget/globalgraphicsitem.cpp \
    print/print.cpp \
    communication/ftpmanager.cpp \
    communication/ftpconfiguration.cpp \
    database/operatorinfo.cpp \
    jsonEdit/transportinfo.cpp \
    communication/indicatorlightcom.cpp \
    communication/indicatorlightconfiguration.cpp \
    architePlan/firstfirealarminfowidget.cpp \
    customTimer/customtimer.cpp \
    LogMsg/datapool.cpp \
    LogMsg/debuglogmsg.cpp \
    LogMsg/errorlogmsg.cpp \
    LogMsg/infologmsg.cpp \
    LogMsg/logmsg.cpp \
    communication/tcpserver.cpp


HEADERS += \
        crtwidget.h \
    architePlan/architeplanview.h \
    infoTable/infotableview.h \
    graphicsWidget/graphicsscene.h \
    graphicsWidget/graphicsview.h \
    graphicsWidget/graphicsitem.h \
    architePlan/treeview.h \
    jsonEdit/jsonedit.h \
    control/controller.h \
    dataStore/datastore.h \
    speech/speechobj.h \
    database/sqlitemanager.h \
    database/sqlmanager.h \
    architePlan/sysarchiteplanview.h \
    qmlTableModel/qmltablemodel.h \
    jsonEdit/qmlforjson.h \
    control/usermanager.h \
    jsonEdit/itemiconinfotojson.h \
    architePlan/styleditemdelegate.h \
    communication/abstractconfiguration.h \
    communication/abstractlink.h \
    communication/seriallink.h \
    communication/tcplink.h \
    communication/serialconfiguration.h \
    communication/tcpconfiguration.h \
    communication/configurationmanager.h \
    communication/udplink.h \
    communication/udpconfiguration.h \
    communication/canconfiguration.h \
    database/mysqlmanager.h \
    dataStore/abstractdataprotocol.h \
    dataStore/serialdataprotocol.h \
    dataStore/indicatordataprotocol.h \
    dataStore/monitoringprotocol.h \
    graphicsWidget/globalgraphicsview.h \
    graphicsWidget/globalgraphicsscene.h \
    graphicsWidget/globalgraphicsitem.h \
    print/print.h \
    communication/ftpmanager.h \
    communication/ftpconfiguration.h \
    database/operatorinfo.h \
    jsonEdit/transportinfo.h \
    communication/indicatorlightcom.h \
    communication/indicatorlightconfiguration.h \
    architePlan/firstfirealarminfowidget.h \
    customTimer/customtimer.h \
    LogMsg/datapool.h \
    LogMsg/debuglogmsg.h \
    LogMsg/errorlogmsg.h \
    LogMsg/infologmsg.h \
    LogMsg/logmsg.h \
    communication/tcpserver.h

FORMS +=

DISTFILES += \
    package.pri

RC_ICONS = logo_icon.ico

RESOURCES += \
    images.qrc \
    qss.qrc \
    qml.qrc

include(package.pri)

