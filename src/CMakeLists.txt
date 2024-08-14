add_executable(kode-reviewer)

add_definitions(-fexceptions)
add_definitions(-O0)
add_definitions(-DQT_QML_GENERATE_QMLLS_INI=ON)
set(QT_QML_GENERATE_QQMLLS_INI ON)
remove_definitions(-DQT_NO_KEYWORDS)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

#Remove -mno-direct-extern-access for clang
string(REPLACE "-mno-direct-extern-access" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

ecm_add_qml_module(kode-reviewer
    URI
    org.kde.kodereviewer
)

target_sources(kode-reviewer
    PRIVATE
    main.cpp
    pullrequestmodel.cpp
    networkmanager.cpp
    commentsmodel.cpp
    apidata.cpp

    linemodel.cpp

    gitbackend.cpp
    libgit/repository.cpp
    libgit/gitobject.cpp
    libgit/utils.cpp
    libgit/commit.cpp
    libgit/tree.cpp
    libgit/diff.cpp
)

ecm_target_qml_sources(kode-reviewer
    SOURCES
    qml/Main.qml
    qml/ProjectDrawer.qml

    qml/PullRequestOverviewPage.qml

    qml/components/RoundedImage.qml
    qml/components/CommentControl.qml
)

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
    LibGit2::LibGit2
)

install(TARGETS kode-reviewer  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


## Some tests
add_executable(git-test)
target_sources(git-test
    PRIVATE
    git-test.cpp
    libgit/repository.cpp
    libgit/gitobject.cpp
    libgit/utils.cpp
    libgit/commit.cpp
    libgit/tree.cpp
    libgit/diff.cpp
)

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
  LibGit2::LibGit2
)
