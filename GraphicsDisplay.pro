#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T10:35:22
#
#-------------------------------------------------

QT       += core gui quick svg opengl concurrent sql texttospeech serialport

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
    openglWidget/glwidget.cpp \
    control/controller.cpp \
    dataStore/datastore.cpp \
    speech/speechobj.cpp \
    commnication/commobj.cpp \
    database/sqlitemanager.cpp \
    database/sqlmanager.cpp \
    architePlan/sysarchiteplanview.cpp \
    qmlTableModel/qmltablemodel.cpp \
    jsonEdit/qmlforjson.cpp \
    control/usermanager.cpp \
    jsonEdit/itemiconinfotojson.cpp \
    graphicsWidget/svgrenderer.cpp

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
    commnication/commobj.h \
    database/sqlitemanager.h \
    database/sqlmanager.h \
    architePlan/sysarchiteplanview.h \
    qmlTableModel/qmltablemodel.h \
    jsonEdit/qmlforjson.h \
    control/usermanager.h \
    jsonEdit/itemiconinfotojson.h \
    graphicsWidget/svgrenderer.h

FORMS +=

DISTFILES += \
    qml/SerialPortSetting.qml \
    qml/GraphicsViewSetting.qml \
    qml/NaviButton.qml \
    qml/SettingWindow.qml \
    qml/PasswordSetting.qml \
    qml/SysArchitePlanSetting.qml \
    qml/SysArchitePlanSetting.qml \
    qml/SysArchitePlanSetting.qml \
    qml/InfoQuery.qml \
    qml/InfoTransport.qml \
    qml/OtherSetting.qml \
    qml/qmlTable/AlarmInfoTableItem.qml \
    qml/qmlTable/DeviceStateTableItem.qml \
    qml/qmlTable/OperaEventTableItem.qml

RESOURCES += \
    qml.qrc \
    images.qrc \
    qss.qrc
