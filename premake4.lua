#!lua

solution "TileEngine"
  configurations { "Debug", "Release" }
  buildoptions { "-std=c++11", "-Wno-unknown-pragmas", "-g", "-O0", '-pthread'}

  project "TileEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "TileEngine"
    targetextension ".out"

    libdirs { "./lib" }
    links { "Box2d", "GLUI", "fmod", "glfw3", "OpenGL.framework", "png", "z", "tmxparser", "IL"}

    includedirs { "./include", "./src" }

    files { "**.h", "**.hpp", "**.cpp" }
    excludes { "./test/**" }

    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }

  project "TileEngineTests"
    kind "ConsoleApp"
    language "C++"

    targetname "TileEngineTests"
    targetextension ".out"

    libdirs { "./lib" }
    links { "Box2d", "GLUI", "fmod", "glfw3", "OpenGL.framework", "png", "z", "tmxparser", "IL"}

    includedirs { "./include", "./src" }

    files { "**.h", "**.hpp", "**.cpp" }
    excludes { "./src/main.cpp" }

    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }
