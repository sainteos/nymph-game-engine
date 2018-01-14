#!lua

solution "TileEngine"
  configurations { "Debug", "Release", "WindowsDebug" }

  project "TileEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "TileEngine"

    includedirs { "./include", "./ext/nanogui/include", "./ext/nanogui/ext/eigen", "./ext/nanogui/ext/nanovg/src", "./ext/nanogui/ext/glfw/include", "./src" }

    files { "**.h", "**.hpp", "**.cpp", "**.cc" }
    excludes { "./test/**", "./ext/**" }

    libdirs { "./ext/nanogui" }

    configuration "WindowsDebug"
      buildoptions { "-std=c++11" }
      links { "freetype_win", "yse", "glfw3", "gdi32", "opengl32", "png", "z", "DevIL", "chaiscript_stdlib-5.8.5" }
      defines { "DEBUG" }
      targetextension ".exe"

    configuration "Debug"
      buildoptions { "-std=c++14", "-Wno-unknown-pragmas", "-g", "-O0", '-pthread' }
      links {  "freetype", "yse", "nanogui", "png", "z", "OpenGL.framework", "IL" }
      defines { "DEBUG" }
      flags { "Symbols" }
      targetextension ".out"

    configuration "Release"
      buildoptions { "-std=c++11", "-Wno-unknown-pragmas", "-g", "-O0", '-pthread' }
      links {  "freetype", "yse", "glfw3.3.1", "png", "z", "OpenGL.framework", "IL", "chaiscript_stdlib-5.8.5" }
      defines { "NDEBUG" }
      flags { "Optimize" }
      targetextension ".out"
