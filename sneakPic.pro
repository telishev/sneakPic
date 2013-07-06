DISTFILES += sneakPic.pro
TARGET = sneakPic
TEMPLATE = app
QT += core widgets svg xml opengl

isEmpty ($$PROJ_DIR) {
  PROJ_DIR = $$PWD
}

include (src/files.in)

# configuration

#RC_FILE += $$PROJ_DIR/resource/sneakPic.rc
#RESOURCES += $$PROJ_DIR/resource/sneakPic.qrc

CONFIG += warn_on build_all debug_and_release

INCLUDEPATH += src
DEPENDPATH  += src
VPATH += src

# These are for gengetopt, version should be updated correctly later though
DEFINES += "PACKAGE=\"sneakPic\""
DEFINES += "VERSION=\"0.01\""

UI_DIR = $$PROJ_DIR/src/ui

win32:*msvc*{
  QMAKE_CXXFLAGS_DEBUG += /MP -wd4996 -we4062 /W4 /WX /Zc:wchar_t-
  QMAKE_CXXFLAGS_RELEASE += /MP -wd4996 /W4 /Ob2 /Oi /Ot /Oy /GS- /GF /Gy /WX /Zc:wchar_t-

}

Release {
  OBJECTS_DIR = $$PROJ_DIR/build
  MOC_DIR = $$PROJ_DIR/build
  RCC_DIR = $$PROJ_DIR/build
  DESTDIR = $$PROJ_DIR/build
}

Debug {
  OBJECTS_DIR = $$PROJ_DIR/build.dbg
  MOC_DIR = $$PROJ_DIR/build.dbg
  RCC_DIR = $$PROJ_DIR/build.dbg
  DESTDIR = $$PROJ_DIR/build.dbg
}
