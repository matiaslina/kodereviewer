cmake_minimum_required(VERSION 3.20)
project(kode-reviewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
# string(REPLACE "-mno-direct-extern-access" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})


add_subdirectory(libgit)

add_executable(kode-reviewer resources.qrc $<TARGET_OBJECTS:libgit>)

add_definitions(-DQT_QML_GENERATE_QMLLS_INI=ON)
set(QT_QML_GENERATE_QQMLLS_INI ON)
remove_definitions(-DQT_NO_KEYWORDS)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

target_compile_options(kode-reviewer INTERFACE "$<$<CXX_COMPILER_ID:GNU>:-mno-direct-extern-access>")
target_compile_options(kode-reviewer INTERFACE "$<$<CXX_COMPILER_ID:Clang>:-fno-direct-access-external-data>")

ecm_add_qml_module(kode-reviewer
    URI
    org.kde.kodereviewer
)


target_sources(kode-reviewer
    PRIVATE
    main.cpp

    apidata.cpp

    # models
    linemodel.cpp
    pullrequestmodel.cpp
    commentsmodel.cpp
    filemodel.cpp

    # Backends
    gitbackend.cpp
    networkmanager.cpp

)

ecm_target_qml_sources(kode-reviewer
    SOURCES
    qml/Main.qml
    qml/ProjectDrawer.qml

    qml/PullRequestOverviewPage.qml
    qml/ReviewFilePage.qml

    qml/components/DrawerHeader.qml
    qml/components/RoundedImage.qml
    qml/components/CommentControl.qml
    qml/components/CommentDelegate.qml
    qml/components/FileList.qml
    qml/components/FileDelegate.qml
    qml/components/PullRequestOverview.qml
    qml/components/Editor.qml
)

target_link_libraries(kode-reviewer
    PRIVATE
    libgit
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
    LibGit2::LibGit2
)

install(TARGETS kode-reviewer  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


## Some tests
add_executable(git-test $<TARGET_OBJECTS:libgit>)
target_sources(git-test
    PRIVATE
    git-test.cpp
)

target_link_libraries(git-test
  PRIVATE
  libgit
  Qt6::Quick
  Qt6::Qml
  Qt6::Gui
  Qt6::QuickControls2
  Qt6::Widgets
  KF6::I18n
  KF6::CoreAddons
  KF6::IconThemes
  LibGit2::LibGit2
)
