import qbs 1.0
// import 'src/files.qbs' as Files
// import "skia.qbs" as SkiaConfig

Application {
    name : "sneakPic"
    destinationDirectory: "bin"
  consoleApplication: false

    Depends {
        name: "Qt"
        submodules: ["core", "widgets", "svg", "xml", "opengl", "gui"]
    }
  Depends { name: 'cpp'
  }

  cpp.defines:{

    var defines = [
    'NOMINMAX=1',
    'VERSION="0.01"',
    'PACKAGE="sneakPic"'
    ];

    if (qbs.toolchain.contains ('msvc'))
      defines.push ('_VARIADIC_MAX=10');

    if (qbs.buildVariant === 'debug')
      defines.push ("SK_DEBUG");
    else
      defines.push ("SK_RELEASE");

    if (qbs.targetOS.contains ('windows'))
    {
      defines = defines.concat ("_WINDOWS", "WIN32", "WIN64");
    }

      return defines;
  }

  cpp.cxxFlags :
    {
    var flags = [];
    if (qbs.buildVariant === 'debug')
      {
        if (qbs.toolchain.contains ('msvc'))
        {
          flags = ["/wd4996", "/we4062", "/W3", "/WX" ,"/Zc:wchar_t-", "/Gm-", "/w34100", "/w34189", "/Fd" + product.buildDirectory + "sneakPic.pdb"];
        }
      }
    else
      {
        if (qbs.toolchain.contains ('msvc'))
          flags = ["/MP", "-wd4996", "/W3", "/Ob2", "/Oi", "/Ot", "/Oy", "/GS-", "/GF", "/Gy", "/WX", "/Zc:wchar_t-", "/w34100", "/w34189"]
      }
      if (qbs.toolchain.contains ("gcc"))
        flags.push ('-std=c++11');

      return flags;
    }

  cpp.linkerFlags:
   {
    var flags = [];
    if (qbs.toolchain.contains ("msvc") && qbs.buildVariant === "debug")
      flags.push ('/INCREMENTAL');
    return flags;
   }

  property string baseDir: 'src/'
  property string skia_dir: qbs.getenv('SKIA_DIR') || ''

  cpp.includePaths:
  {
    var paths = [baseDir, baseDir + 'gtest'];
    if (skia_dir.length !== 0)
      {
        paths = paths.concat ([skia_dir + '/include/core/', skia_dir + '/include/config/', skia_dir + '/include/effects/']);
      }
    else
      throw new Error ("Please, specify skia dir using environment variable SKIA_DIR");
    return paths;
  }

  cpp.cxxPrecompiledHeader: "src/precompiled/precompiled_header.h"

  cpp.libraryPaths:
   {
    var paths;
    if (qbs.buildVariant === 'debug')
      paths = [skia_dir + '/out/Debug', skia_dir + '/out/Debug/lib']
    else
      paths = [skia_dir + '/out/Release', skia_dir + '/out/Release/lib']
    return paths;
   }

  cpp.staticLibraries:
    {
      var libs;
      if (qbs.toolchain.contains ('msvc'))
      {
        libs = ['usp10.lib', 'skia_core.lib', 'skia_images.lib', 'skia_opts.lib', 'skia_effects.lib', 'skia_pdf.lib', 'skia_opts_ssse3.lib',
              'zlib.lib', 'skia_skgpu.lib', 'skia_utils.lib', 'skia_ports.lib', 'skia_animator.lib', 'skia_sfnt.lib', 'opengl32.lib', 'gdi32.lib', 'user32.lib' ,'Ole32.lib', 'kernel32.lib'];
      } else if (qbs.toolchain.contains ("gcc"))
      {
        libs = ['-Wl', '--start-group', '-lskia_images', '-lskia_opts_ssse3', '-lskia_animator', '-lskia_skgpu', '-lskia_xml', '-lskia_core',
            '-lskia_effects', '-lskia_pdf', '-lskia_ports', '-lskia_utils', '-lskia_sfnt', '-lskia_opts', '-lskia_views', '-Wl', '--end-group',
            '-lpthread', '-lgif', '-lpng', '-lz', '-lfontconfig', '-ldl', '-lfreetype', '-lGL', '-lGLU', '-lX11']
      }

      return libs;
    }

  Group {
        name: "resource"
        prefix: name + '/'
        files: [
            "sneakPic.qrc",
        ]
    }

  Group {
        name: "ui"
        files: [
            "src/ui/main_window.ui"
        ]
    }

  Group {
        name: "everything"
        prefix: baseDir
        files: [
            "**/*.cpp",
      "**/*.h",
        ]
    }

  Group {
        files: [
            "src/gtest/gmock-gtest-all.cc"
        ]
    }
}
