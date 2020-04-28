#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T15:39:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dist
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    contarparticulas.cpp \
    dialogbrilhocontraste.cpp \
    dialogcontagemautomatica.cpp \
    dialogoplogicasaritmeticas.cpp \
    dialogsegmentacao.cpp \
    paint.cpp \
    imageoperations.cpp \
    estereologia.cpp

HEADERS  += mainwindow.h \
    contarparticulas.h \
    dialogbrilhocontraste.h \
    dialogcontagemautomatica.h \
    dialogoplogicasaritmeticas.h \
    dialogsegmentacao.h \
    paint.h \
    imageoperations.h \
    estereologia.h

FORMS    += mainwindow.ui \
    dialogbrilhocontraste.ui \
    dialogcontagemautomatica.ui \
    dialogoplogicasaritmeticas.ui \
    dialogsegmentacao.ui

INCLUDEPATH += C:\opencv\build\include

LIBS += C:\opencv-build\bin\libopencv_core420.dll
LIBS += C:\opencv-build\bin\libopencv_highgui420.dll
LIBS += C:\opencv-build\bin\libopencv_imgcodecs420.dll
LIBS += C:\opencv-build\bin\libopencv_imgproc420.dll
LIBS += C:\opencv-build\bin\libopencv_features2d420.dll
LIBS += C:\opencv-build\bin\libopencv_calib3d420.dll

RESOURCES += \
    myresource.qrc

RC_FILE = icone.rc
