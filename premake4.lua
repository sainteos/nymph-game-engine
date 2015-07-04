#!lua

solution "TileEngine"
  configurations { "Debug", "Release" }

  project "TileEngine"
    kind "ConsoleApp"
    language "C++"

    targetname "TileEngine"
    targetextension ".out"

    libdirs { "lib" }
    links { "Box2d", "GLUI", "SOIL", "fmod", "log4cplus" }

    files { "**.h", "**.hpp", "**.cpp" }

    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }