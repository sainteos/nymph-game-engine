#!lua

solution "NymphGameEngine"
  configurations { "MacDebug", "MacRelease", "WindowsDebug", "WindowsRelease", "LinuxDebug", "LinuxRelease" }

  project "NymphGameEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "NymphGameEngine"

    includedirs { "./include", "./src" }

    files { "**.h", "**.hpp", "**.cpp", "**.cc" }
    excludes { "./test/**" }

    configuration "MacDebug"
      buildoptions { "-std=c++14",  "-g", "-O0", '-pthread', "`freetype-config --cflags`" }
      links {  "freetype", "glfw", "png", "z", "OpenGL.framework", "glew", "IL", "tmxparser", "dl" }
      defines { "DEBUG" }
      flags { "Symbols" }
      targetextension ".out"

    configuration "MacRelease"
      buildoptions { "-std=c++14", "-pthread" }
      links {  "freetype", "glfw", "png", "z", "OpenGL.framework", "glew", "IL", "tmxparser", "dl", "`freetype-config --cflags`" }
      defines { "NDEBUG" }
      flags { "Optimize" }
      targetextension ".out"

    configuration "LinuxDebug"
      buildoptions { "-std=c++14", "-g", "-O0", "-pthread", "`freetype-config --cflags`" }
      links {  "freetype", "glfw", "png", "GLEW", "IL", "tmxparser", "chaiscript_stdlib-6.0.0", "z", "dl", "GL", "pthread" }
      defines { "DEBUG" }
      flags { "Symbols" }
      targetextension ".out"

    configuration "LinuxRelease"
      buildoptions { "-std=c++14", "-pthread", "`freetype-config --cflags`" }
      links {  "freetype", "glfw", "png", "GLEW", "IL", "tmxparser", "chaiscript_stdlib-6.0.0", "z", "dl", "GL", "pthread" }
      defines { "NDEBUG" }
      flags { "Optimize" }
      targetextension ".out"

    configuration "WindowsDebug"
      buildoptions { "-std=c++1y", "`freetype-config --cflags`" }
      links { "freetype", "glfw3", "gdi32", "GLEW", "opengl32", "png", "z", "DevIL", "chaiscript_stdlib-6.0.0" }
      defines { "DEBUG", "GLEW_STATIC" }
      targetextension ".exe"

    configuration "WindowsRelease"
      buildoptions { "-std=c++1y", "`freetype-config --cflags`" }
      links { "freetype", "glfw3", "gdi32", "GLEW", "opengl32", "png", "z", "DevIL", "chaiscript_stdlib-6.0.0" }
      defines { "DEBUG", "GLEW_STATIC" }
      targetextension ".exe"
