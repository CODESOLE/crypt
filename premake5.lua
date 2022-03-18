workspace "crypt"
  configurations { "Debug", "Release" }
  platforms { "x64" }
  location "build"

project "crypt"
  kind "ConsoleApp"
  language "C"
  cdialect "C99"
  warnings "Extra"
  targetname "crypt"
  targetdir "build"

  buildoptions { "-pedantic" }
  includedirs { "src", "dep" }

  files { "src/**.c", "dep/**.c" }
  links { "ncurses" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
    optimize "Off"

  filter "configurations:Release"
    defines { "NDEBUG" }
    symbols "Off"
    optimize "Full"
