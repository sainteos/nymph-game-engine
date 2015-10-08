#!lua

solution "TileEngine"
  configurations { "Debug", "Release", "WindowsDebug" }

  project "TileEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "TileEngine"

    libdirs { "./lib" }

    includedirs { "./include", "./src" }

    files { "**.h", "**.hpp", "**.cpp" }
    excludes { "./test/**" }

    configuration "WindowsDebug"
      buildoptions { "-std=c++11" }
      links { "freetype_win", "fmod", "glfw3", "gdi32", "glew32s", "opengl32", "png", "z", "DevIL" }
      defines { "DEBUG", "GLEW_STATIC" }
      targetextension ".exe"

    configuration "Debug"
      buildoptions { "-std=c++11", "-Wno-unknown-pragmas", "-g", "-O0", '-pthread' }
      links {  "freetype", "fmod", "glfw3.3.1", "png", "z", "OpenGL.framework", "glew", "IL" }
      defines { "DEBUG" }
      flags { "Symbols" }
      targetextension ".out"

    configuration "Release"
      buildoptions { "-std=c++11", "-Wno-unknown-pragmas", "-g", "-O0", '-pthread' }
      links {  "freetype", "fmod", "glfw3.3.1", "png", "z", "OpenGL.framework", "glew", "IL" }
      defines { "NDEBUG" }
      flags { "Optimize" }
      targetextension ".out"

