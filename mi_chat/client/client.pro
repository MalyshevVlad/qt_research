QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    authorization.cpp \
    choicecreate.cpp \
    confirmwindow.cpp \
    creategroup.cpp \
    emojiframe.cpp \
    findcontact.cpp \
    main.cpp \
    client.cpp \
    registration.cpp \
    selectcontacts.cpp \
    usersgroupinfo.cpp

HEADERS += \
    aboutdialog.h \
    authorization.h \
    choicecreate.h \
    client.h \
    confirmwindow.h \
    creategroup.h \
    emojiframe.h \
    findcontact.h \
    registration.h \
    selectcontacts.h \
    usersgroupinfo.h

FORMS += \
    aboutdialog.ui \
    authorization.ui \
    choicecreate.ui \
    client.ui \
    confirmwindow.ui \
    creategroup.ui \
    emojiframe.ui \
    findcontact.ui \
    registration.ui \
    selectcontacts.ui \
    usersgroupinfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
