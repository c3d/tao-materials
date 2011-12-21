# ******************************************************************************
#  materials.pro                                                     Tao project
# ******************************************************************************
# File Description:
# Qt build file for the Materials module
# ******************************************************************************
# This software is property of Taodyne SAS - Confidential
# Ce logiciel est la propriété de Taodyne SAS - Confidentiel
# (C) 2010 Jerome Forissier <jerome@taodyne.com>
# (C) 2010 Taodyne SAS
# ******************************************************************************

MODINSTDIR = materials

include(../modules.pri)

OTHER_FILES = materials.xl materials.tbl traces.tbl

DEFINES     += GLEW_STATIC

INCLUDEPATH += $${TAOTOPSRC}/tao/include/tao/
HEADERS = \
    fresnel.h \
    materials.h \
    material.h \
    plastic.h \
    wood.h  \
    granite.h \
    marble.h \

SOURCES = $${TAOTOPSRC}/tao/include/tao/GL/glew.c \
    fresnel.cpp \
    materials.cpp \
    material.cpp \
    plastic.cpp \
    wood.cpp \
    granite.cpp \
    marble.cpp \

TBL_SOURCES  = materials.tbl

QT          += core \
               gui \
               opengl

INSTALLS    += thismod_icon

LICENSE_FILES = materials.taokey.notsigned
include(../licenses.pri)

QMAKE_SUBSTITUTES = doc/Doxyfile.in
DOXYFILE = doc/Doxyfile
include(../modules_doc.pri)
