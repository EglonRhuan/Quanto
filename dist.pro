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

INCLUDEPATH += C:\\opencv-3.1.0\\opencv\\build\\include

LIBS += -LC:\\opencv-3.1.0\\mybuild\\lib\\Debug \
    -lopencv_imgcodecs310d \
    -lopencv_imgproc310d \
    -lopencv_ml310d \
    -lopencv_objdetect310d \
    -lopencv_photo310d \
    -lopencv_shape310d \
    -lopencv_stitching310d \
    -lopencv_superres310d \
    -lopencv_ts310d \
    -lopencv_video310d \
    -lopencv_videoio310d \
    -lopencv_videostab310d \
    -lopencv_calib3d310d \
    -lopencv_core310d \
    -lopencv_features2d310d \
    -lopencv_flann310d \
    -lopencv_highgui310d

RESOURCES += \
    myresource.qrc

RC_FILE = icone.rc
