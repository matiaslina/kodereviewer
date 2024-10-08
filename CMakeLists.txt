cmake_minimum_required(VERSION 3.20)
project(kode-reviewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(QT_QML_GENERATE_QQMLLS_INI ON)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Test
    Gui
    QuickControls2
    Widgets
)

find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    I18n
    CoreAddons
    QQC2DesktopStyle
    IconThemes
)

find_package(LibGit2 REQUIRED)

# LibQGit2
include(${CMAKE_HOME_DIRECTORY}/Modules/FindLibQGit2.cmake)
include(FindOpenGL)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

add_subdirectory(src)

install(PROGRAMS kode-reviewer.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kode-reviewer.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES kode-reviewer.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)
