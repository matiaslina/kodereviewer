cmake_minimum_required(VERSION 3.20)
project(libgit)

string(REPLACE "-mno-direct-extern-access" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

remove_definitions(-DQT_NO_KEYWORDS)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)


add_library(libgit
  commit.cpp
  blob.cpp
  diff.cpp
  gitobject.cpp
  repository.cpp
  tree.cpp
  utils.cpp
)
target_link_libraries(libgit
  PRIVATE
  Qt6::Core
  Qt6::Qml
  Qt6::Gui
  Qt6::QuickControls2
  Qt6::Widgets
  LibGit2::LibGit2
)
