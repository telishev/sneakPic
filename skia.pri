DISTFILES += skia.pri

QMAKE_SKIA_DIR = $$(SKIA_DIR)

exists ($$QMAKE_SKIA_DIR) {
  INCLUDEPATH += $$QMAKE_SKIA_DIR/include/core/
  INCLUDEPATH += $$QMAKE_SKIA_DIR/include/config/
  INCLUDEPATH += $$QMAKE_SKIA_DIR/include/effects/
  Release {
    QMAKE_LIBDIR += $$QMAKE_SKIA_DIR/out/Release
    QMAKE_LIBDIR += $$QMAKE_SKIA_DIR/out/Release/lib
  }
  Debug {
    QMAKE_LIBDIR += $$QMAKE_SKIA_DIR/out/Debug
    QMAKE_LIBDIR += $$QMAKE_SKIA_DIR/out/Debug/lib
  }

} else {
  message ("Please, specify skia dir using environment variable SKIA_DIR")
}

win32:*msvc* {
  LIBS *=  usp10.lib skia_core.lib skia_images.lib skia_opts.lib skia_effects.lib skia_pdf.lib skia_opts_ssse3.lib \
           zlib.lib skia_skgpu.lib skia_utils.lib skia_ports.lib skia_animator.lib skia_sfnt.lib opengl32.lib
}
*g++* {
  LIBS *= -Wl,--start-group \
            -lskia_images -lskia_opts_ssse3 -lskia_animator -lskia_skgpu -lskia_xml -lskia_core \
            -lskia_effects -lskia_pdf -lskia_ports -lskia_utils -lskia_sfnt -lskia_opts -lskia_views \
          -Wl,--end-group \
          -lpthread -lgif -lpng -lz -lfontconfig -ldl -lfreetype -lGL -lGLU -lX11
}
