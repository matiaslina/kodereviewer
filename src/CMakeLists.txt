cmake_minimum_required(VERSION 3.20)
project(kode-reviewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
# string(REPLACE "-mno-direct-extern-access" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

include_directories($ENV{HOME}/.local/include)
link_directories($ENV{HOME}/.local/lib)

add_executable(kode-reviewer resources.qrc)

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
    projectmodel.cpp
    diffmodel.cpp

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
    qml/SettingsPage.qml
    qml/WelcomePage.qml

    qml/CommentWindow.qml

    qml/components/DrawerHeader.qml
    qml/components/RoundedImage.qml
    qml/components/CommentControl.qml
    qml/components/CommentDelegate.qml
    qml/components/FileList.qml
    qml/components/FileDelegate.qml
    qml/components/PullRequestOverview.qml
    qml/components/Editor.qml
)

target_compile_options(kode-reviewer PRIVATE -fexceptions -Wno-zero-as-null-pointer-constant)

target_link_libraries(kode-reviewer
    PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
    qgit2
)

install(TARGETS kode-reviewer  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_executable(git-test)
target_sources(git-test PRIVATE git-test.cpp)
target_compile_options(git-test  PRIVATE -fexceptions -Wno-zero-as-null-pointer-constant)
target_link_libraries(git-test
      PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
    git2
    qgit2
  )
