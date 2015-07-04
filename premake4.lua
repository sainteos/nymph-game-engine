#!lua

solution "TileEngine"
  configurations { "Debug", "Release" }
  buildoptions { "-std=c++11" }

  project "TileEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "TileEngine"
    targetextension ".out"

    libdirs { "./lib" }
    links { "Box2d", "GLUI", "SOIL", "fmod" }

    includedirs { "./include" }

    files { "**.h", "**.hpp", "**.cpp" }
    excludes { "test/*" }

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
    links { "Box2d", "GLUI", "SOIL", "fmod" }

    includedirs { "./include" }

    files { "**.h", "**.hpp", "**.cpp" }
    excludes { "src/*" }

    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }
