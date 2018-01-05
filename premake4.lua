#!lua

solution "NymphGameEngine"
  configurations { "Debug", "Release", "WindowsDebug", "WindowsRelease" }

  project "NymphGameEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "NymphGameEngine"

    includedirs { "./include", "./src" }

    files { "**.h", "**.hpp", "**.cpp", "**.cc" }
    excludes { "./test/**" }

    configuration "Debug"
      buildoptions { "-std=c++14",  "-g", "-O0", '-pthread' }
      links {  "freetype", "yse", "glfw", "png", "z", "OpenGL.framework", "glew", "IL", "tmxparser", "chaiscript" }
      defines { "DEBUG" }
      flags { "Symbols" }
      targetextension ".out"

    configuration "Release"
      buildoptions { "-std=c++14", "-pthread" }
      links {  "freetype", "yse", "glfw", "png", "z", "OpenGL.framework", "glew", "IL", "tmxparser", "chaiscript" }
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
