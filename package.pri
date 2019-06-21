
linux
{
    linux-g++ | linux-g++-64 | linux-g++-32 | linux-clang
    {
        CONFIG  += LinuxBuild
       message("LinuxBuild")
    }
    linux-rasp-pi2-g++ {
        CONFIG += LinuxBuild
        DEFINES +=  __rasp_pi2__
        message("LinuxBuild")
    }
}

LinuxBuild {

   QMAKE_POST_LINK += mkdir -p $$DESTDIR

    message("LinuxBuild")
    # QT_INSTALL_LIBS
    QT_LIB_LIST += \
        libQt5Core.so.5 \
        libQt5DBus.so.5 \
        libQt5Gui.so.5 \
        libQt5Network.so.5 \
        libQt5OpenGL.so.5 \
        libQt5PrintSupport.so.5 \
        libQt5Qml.so.5 \
        libQt5Quick.so.5 \
        libQt5QuickControls2.so.5 \
        libQt5QuickTemplates2.so.5 \
        libQt5SerialPort.so.5 \
        libQt5Sql.so.5 \
        libQt5Svg.so.5 \
        libQt5Widgets.so.5 \
        libQt5XcbQpa.so.5 \
        libQt5TextToSpeech.so.5\
        libQt5Concurrent.so.5


    !contains(DEFINES, __rasp_pi2__) {
        QT_LIB_LIST += \
            libicudata.so.56 \
            libicui18n.so.56 \
            libicuuc.so.56
    }

    for(QT_LIB, QT_LIB_LIST) {
        QMAKE_POST_LINK += && $$QMAKE_COPY --dereference $$[QT_INSTALL_LIBS]/$$QT_LIB $$DESTDIR
    }

    # QT_INSTALL_PLUGINS
    QT_PLUGIN_LIST += \
        bearer \
        iconengines \
        imageformats \
        platforminputcontexts \
        platforms \
        sqldrivers \
        texttospeech\
        qmltooling\
        printsupport

   # QT_INSTALL_QML
    QT_QML_LIST += \
          Qt\
          QtQuick\
          QtQuick.2\
          QtQml\
          QtGraphicalEffects



    !contains(DEFINES, __rasp_pi2__) {
        QT_PLUGIN_LIST += xcbglintegrations
    }


    for(QT_PLUGIN, QT_PLUGIN_LIST) {
        QMAKE_POST_LINK += && $$QMAKE_COPY --dereference --recursive $$[QT_INSTALL_PLUGINS]/$$QT_PLUGIN $$DESTDIR
    }

    # QT_INSTALL_QML
     for(QT_QML, QT_QML_LIST){
      QMAKE_POST_LINK += && $$QMAKE_COPY --dereference --recursive $$[QT_INSTALL_QML]/$$QT_QML $$DESTDIR
    }
}

