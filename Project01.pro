QT       += core gui widgets quickwidgets network positioning location

greaterThan(QT_MAJOR_VERSION, 4):

DEFINES+=CURPATH="\"$$PWD\""
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutproject.cpp \
    main.cpp \
    mainwindow.cpp\
    point.cpp \
    recognizeprocess.cpp \
    server.cpp\
    packet.cpp\
    pipe.cpp\

HEADERS += \
    aboutproject.h \
    mainwindow.h \
    point.h \
    recognizeprocess.h \
    server.h\
    packet.h\
    socketposition.h\
    pipe.h\
FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc

FORMS += \
    aboutproject.ui \
    mainwindow.ui

DISTFILES += \
    recognizer.exe
