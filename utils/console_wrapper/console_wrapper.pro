DISTFILES += console_wrapper.pro
TARGET = sneakPic
TARGET_EXT = .com
TEMPLATE = app
QT += core widgets svg xml opengl

isEmpty ($$PROJ_DIR) {
  PROJ_DIR = $$PWD
}

CONFIG += warn_on debug_and_release console

win32:*msvc*{
  QMAKE_CXXFLAGS_RELEASE += /MP -wd4996 /W4 /Ob2 /Oi /Ot /Oy /GS- /GF /Gy /WX /Zc:wchar_t-
}

SOURCES += console_wrapper.cpp

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
