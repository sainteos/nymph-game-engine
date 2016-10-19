#!/usr/bin/env ruby
`ruby script_registration_generator.rb`
`premake4 gmake`
makefile = File.read("TileEngine.make")
makefile.gsub!("CC = gcc", "CC = $(CROSS)gcc")
makefile.gsub!("CXX = g++", "CXX = $(CROSS)g++")
makefile.gsub!("AR = ar", "AR = $(CROSS)ar")
File.new("TileEngine.make", "w") << makefile