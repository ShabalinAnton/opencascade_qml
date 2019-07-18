TEMPLATE = app
QT += qml quick

CONFIG += c++17

QML_IMPORT_PATH =
QML_DESIGNER_IMPORT_PATH =

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

!contains(QMAKE_TARGET.arch, x86_64) {
	ARCH_BITS_SIZE = 32
	DEFINES += _OCC32
} else {
	ARCH_BITS_SIZE = 64
	DEFINES += _OCC64
}

isEmpty(OCCT_INCLUDE_DIR):OCCT_INCLUDE_DIR = $$(CSF_OCCTIncludePath)
isEmpty(OCCT_LIBRARY_DIR):OCCT_LIBRARY_DIR = $$(CSF_OCCTLibPath)

INCLUDEPATH += $$OCCT_INCLUDE_DIR

LIBS += "$$join(OCCT_LIBRARY_DIR, " -L", -L)"

LIBS +=         \
    -lTKernel   \
    -lTKMath    \
    -lTKG3d     \
    -lTKBRep    \
    -lTKGeomBase\
    -lTKGeomAlgo\
    -lTKTopAlgo \
    -lTKPrim    \
    -lTKBO      \
    -lTKBool    \
    -lTKOffset  \
    -lTKService \
    -lTKV3d     \
    -lTKOpenGl  \
    -lTKFillet

win32: LIBS += -luser32 -lopengl32

HEADERS += OcctView.h
	
SOURCES += main.cpp OcctView.cpp

RESOURCES += qml.qrc