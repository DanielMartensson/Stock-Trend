QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addstockwindow.cpp \
    listwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    plotwindow.cpp \
    tools/json/json.cpp \
    tools/yahoofinance/yahoofinance.cpp

HEADERS += \
    addstockwindow.h \
    listwindow.h \
    mainwindow.h \
    plotwindow.h \
    tools/json/json.h \
    tools/yahoofinance/yahoofinance.h

FORMS += \
    addstockwindow.ui \
    listwindow.ui \
    mainwindow.ui \
    plotwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
