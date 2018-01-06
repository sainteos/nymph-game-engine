#!lua

solution "NymphGameEngine"
  configurations { "Debug", "Release", "WindowsDebug", "WindowsRelease", "LinuxDebug", "LinuxRelease" }

  project "NymphGameEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "NymphGameEngine"

    includedirs { "./include", "./src" }

    files { "**.h", "**.hpp", "**.cpp", "**.cc" }
    excludes { "./test/**" }

    configuration "MacDebug"
      buildoptions { "-std=c++14",  "-g", "-O0", '-pthread' }
      links {  "freetype", "yse", "glfw", "png", "z", "OpenGL.framework", "glew", "IL", "tmxparser", "chaiscript" }
      defines { "DEBUG" }
      flags { "Symbols" }
      targetextension ".out"

    configuration "MacRelease"
      buildoptions { "-std=c++14", "-pthread" }
      links {  "freetype", "yse", "glfw", "png", "z", "OpenGL.framework", "glew", "IL", "tmxparser", "chaiscript" }
      defines { "NDEBUG" }
      flags { "Optimize" }
      targetextension ".out"

    configuration "LinuxDebug"
      buildoptions { "-std=c++14", "-lstdc++",  "-g", "-O0", '-pthread' }
      links {  "freetype", "yse", "glfw", "png", "z", "GL", "glew", "IL", "tmxparser", "chaiscript" }
      defines { "DEBUG" }
      flags { "Symbols" }
      targetextension ".out"

    configuration "LinuxRelease"
      buildoptions { "-std=c++14", "-lstdc++", "-pthread" }
      links {  "freetype", "yse", "glfw", "png", "z", "GL", "glew", "IL", "tmxparser", "chaiscript" }
      defines { "NDEBUG" }
      flags { "Optimize" }
      targetextension ".out"

    configuration "WindowsDebug"
      buildoptions { "-std=c++14" }
      links { "freetype_win", "yse", "glfw3", "gdi32", "glew32s", "opengl32", "png", "z", "DevIL", "chaiscript_stdlib-5.8.5" }
      defines { "DEBUG", "GLEW_STATIC" }
      targetextension ".exe"

    configuration "WindowsRelease"
      buildoptions { "-std=c++14" }
      links { "freetype_win", "yse", "glfw3", "gdi32", "glew32s", "opengl32", "png", "z", "DevIL", "chaiscript_stdlib-5.8.5" }
      defines { "DEBUG", "GLEW_STATIC" }
      targetextension ".exe"
