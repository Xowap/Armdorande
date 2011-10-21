#-------------------------------------------------
#
# Project created by QtCreator 2011-10-18T16:03:13
#
#-------------------------------------------------

QT       += core gui network multimedia

TARGET = Armdorande
TEMPLATE = app


SOURCES += main.cpp\
    force.cpp \
    jedi.cpp \
    armdorandetest.cpp \
    lightsaber.cpp \
    armdorande.cpp \
    jedicouncil.cpp \
    intro.cpp \
    arena.cpp

HEADERS  += \
    force.h \
    jedi.h \
    armdorandetest.h \
    lightsaber.h \
    armdorande.h \
    jedicouncil.h \
    intro.h \
    arena.h

FORMS    += \
    armdorandetest.ui \
    jedicouncil.ui


INCLUDEPATH += /home/etudiant/src/armadeus-4.0/buildroot/output/build/cwiid-0.6.00/libcwiid

LIBS += -lcwiid

RESOURCES += \
    geonosis.qrc









unix:!symbian:!maemo5:isEmpty(MEEGO_VERSION_MAJOR) {
    target.path = /opt/Armdorande/bin
    INSTALLS += target
}




