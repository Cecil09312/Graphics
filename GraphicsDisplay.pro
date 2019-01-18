#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T10:35:22
#
#-------------------------------------------------

QT       += core gui quick svg opengl concurrent sql texttospeech serialport serialbus

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
release
{
   OBJECTS_DIR =./obj/release
   MOC_DIR =./moc/release
   RCC_DIR = ./rcc/release
   DESTDIR = ./bin/release
}

debug
{
   OBJECTS_DIR =./obj/debug
   MOC_DIR =./moc/debug
   RCC_DIR = ./rcc/debug
   DESTDIR = ./bin/debug
}

SOURCES += \
        main.cpp \
        crtwidget.cpp \
    architePlan/architeplanview.cpp \
    infoTable/infotableview.cpp \
    graphicsWidget/graphicsscene.cpp \
    graphicsWidget/graphicsview.cpp \
    graphicsWidget/pixmapitem.cpp \
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
    graphicsWidget/svgrenderer.cpp \
    architePlan/styleditemdelegate.cpp \
    commnication/abstractconfiguration.cpp \
    commnication/abstractlink.cpp \
    commnication/seriallink.cpp \
    commnication/tcplink.cpp \
    commnication/serialconfiguration.cpp \
    commnication/tcpconfiguration.cpp \
    commnication/configurationmanager.cpp \
    commnication/udplink.cpp \
    commnication/udpconfiguration.cpp \
    commnication/modbusmanager.cpp \
    commnication/canbasmanager.cpp \
    commnication/canconfiguration.cpp

HEADERS += \
        crtwidget.h \
    architePlan/architeplanview.h \
    infoTable/infotableview.h \
    graphicsWidget/graphicsscene.h \
    graphicsWidget/graphicsview.h \
    graphicsWidget/pixmapitem.h \
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
    graphicsWidget/svgrenderer.h \
    architePlan/styleditemdelegate.h \
    commnication/abstractconfiguration.h \
    commnication/abstractlink.h \
    commnication/seriallink.h \
    commnication/tcplink.h \
    commnication/serialconfiguration.h \
    commnication/tcpconfiguration.h \
    commnication/configurationmanager.h \
    commnication/udplink.h \
    commnication/udpconfiguration.h \
    commnication/modbusmanager.h \
    commnication/canbasmanager.h \
    commnication/canconfiguration.h

FORMS +=

DISTFILES += \


RESOURCES += \
    images.qrc \
    qss.qrc \
    qml.qrc
