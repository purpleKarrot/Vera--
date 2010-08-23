# Copyright (c) 2009 Daniel Pfeifer

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Vera++)

find_package(TCL)
include_directories(${TCL_INCLUDE_PATH})

find_package(Boost REQUIRED COMPONENTS system wave)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(vera++
  src/main.cpp
  src/plugins/cpptcl-1.1.4/cpptcl.cpp
  src/plugins/Exclusions.cpp
  src/plugins/Interpreter.cpp
  src/plugins/Parameters.cpp
  src/plugins/Profiles.cpp
  src/plugins/Reports.cpp
  src/plugins/RootDirectory.cpp
  src/plugins/Rules.cpp
  src/plugins/Transformations.cpp
  src/structures/SourceFiles.cpp
  src/structures/SourceLines.cpp
  src/structures/Tokens.cpp
  )

target_link_libraries(vera++ ${TCL_LIBRARY} ${Boost_LIBRARIES})