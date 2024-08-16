cmake_minimum_required(VERSION 3.20)
project(libgit)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

remove_definitions(-DQT_NO_KEYWORDS)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)


add_library(libgit
  commit.cpp
  blob.cpp
  diff.cpp
  gitobject.cpp
  repository.cpp
  patch.cpp
  tree.cpp
  utils.cpp
)

target_compile_options(libgit INTERFACE "$<$<CXX_COMPILER_ID:GNU>:-mno-direct-extern-access>")
target_compile_options(libgit INTERFACE "$<$<CXX_COMPILER_ID:Clang>:-fno-direct-access-external-data>")

target_link_libraries(libgit
  PRIVATE
  Qt6::Core
  Qt6::Qml
  Qt6::Gui
  Qt6::QuickControls2
  Qt6::Widgets
  LibGit2::LibGit2
)
