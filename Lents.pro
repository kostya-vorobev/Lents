QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    auth.cpp \
    chatclass.cpp \
    lentelem.cpp \
    main.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    newlogin.cpp \
    newselem.cpp \
    postclass.cpp \
    qdialogusers.cpp \
    sendmessageuser.cpp \
    userchatbutton.cpp \
    viewmessage.cpp

HEADERS += \
    auth.h \
    chatclass.h \
    lentelem.h \
    mainwindow.h \
    networkmanager.h \
    newlogin.h \
    newselem.h \
    postclass.h \
    qdialogusers.h \
    sendmessageuser.h \
    userchatbutton.h \
    viewmessage.h

FORMS += \
    auth.ui \
    lentelem.ui \
    mainwindow.ui \
    newlogin.ui \
    newselem.ui \
    qdialogusers.ui \
    sendmessageuser.ui \
    userchatbutton.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
