cmake_minimum_required(VERSION 3.28)

# Project
project(jade_cocoon_randomizer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets)

# Jade Cocoon Unpacker Repacker
set(JCUR_LIB ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/dep/Jade-Cocoon-Unpacker-Repacker)

# mkpsxiso / dumpsxiso
add_subdirectory(${PROJECT_SOURCE_DIR}/dep/mkpsxiso)

# Bin
set(BACKEND_DIR ${PROJECT_SOURCE_DIR}/src/Backend)
set(BACKEND_RANDOMIZER_DIR ${BACKEND_DIR}/Randomizer)
set(COMMON_DIR ${PROJECT_SOURCE_DIR}/src/Common)
set(FRONTENDQT_DIR ${PROJECT_SOURCE_DIR}/src/FrontendQt)

set(SOURCES_BACKEND
    ${BACKEND_DIR}/Behavior.hpp
    ${BACKEND_DIR}/Entity.cpp
    ${BACKEND_DIR}/Entity.hpp
    ${BACKEND_DIR}/File.cpp
    ${BACKEND_DIR}/File.hpp
    ${BACKEND_DIR}/Game.cpp
    ${BACKEND_DIR}/Game.hpp
    ${BACKEND_DIR}/GameTree.cpp
    ${BACKEND_DIR}/GameTree.hpp
    ${BACKEND_DIR}/Iso.cpp
    ${BACKEND_DIR}/Iso.hpp
    ${BACKEND_DIR}/Item.cpp
    ${BACKEND_DIR}/Item.hpp
    ${BACKEND_DIR}/JCUtil.cpp
    ${BACKEND_DIR}/JCUtil.hpp
    ${BACKEND_DIR}/libgte.cpp
    ${BACKEND_DIR}/libgte.hpp
    ${BACKEND_DIR}/Merge.cpp
    ${BACKEND_DIR}/Merge.hpp
    ${BACKEND_DIR}/Mips.hpp
    ${BACKEND_DIR}/MipsFn.hpp
    ${BACKEND_DIR}/Model.cpp
    ${BACKEND_DIR}/Model.hpp
    ${BACKEND_DIR}/Offset.cpp
    ${BACKEND_DIR}/Offset.hpp
    ${BACKEND_DIR}/Ost.cpp
    ${BACKEND_DIR}/Ost.hpp
    ${BACKEND_DIR}/Path.cpp
    ${BACKEND_DIR}/Path.hpp
    ${BACKEND_DIR}/Randomizer.cpp
    ${BACKEND_DIR}/Randomizer.hpp
    ${BACKEND_DIR}/SharedData.cpp
    ${BACKEND_DIR}/SharedData.hpp
    ${BACKEND_DIR}/Tim.cpp
    ${BACKEND_DIR}/Tim.hpp
    ${BACKEND_DIR}/Util.cpp
    ${BACKEND_DIR}/Util.hpp
    ${BACKEND_DIR}/Version.cpp
    ${BACKEND_DIR}/Version.hpp
    ${BACKEND_DIR}/VersionUtil.cpp
    ${BACKEND_DIR}/VersionUtil.hpp)

set(SOURCES_BACKEND_RANDOMIZER
    ${BACKEND_DIR}/Randomizer/Addons.cpp
    ${BACKEND_DIR}/Randomizer/Boss.cpp
    ${BACKEND_DIR}/Randomizer/Challenge.cpp
    ${BACKEND_DIR}/Randomizer/Equipment.cpp
    ${BACKEND_DIR}/Randomizer/Fixes.cpp
    ${BACKEND_DIR}/Randomizer/Forest.cpp
    ${BACKEND_DIR}/Randomizer/Levant.cpp
    ${BACKEND_DIR}/Randomizer/Minion.cpp
    ${BACKEND_DIR}/Randomizer/Misc.cpp
    ${BACKEND_DIR}/Randomizer/Shop.cpp
    ${BACKEND_DIR}/Randomizer/Treasure.cpp)

set(SOURCES_COMMON
    ${COMMON_DIR}/Buffer.hpp
    ${COMMON_DIR}/FileSystem.cpp
    ${COMMON_DIR}/FileSystem.hpp
    ${COMMON_DIR}/JcrException.hpp
    ${COMMON_DIR}/Json.cpp
    ${COMMON_DIR}/Json.hpp
    ${COMMON_DIR}/Random.cpp
    ${COMMON_DIR}/Random.hpp
    ${COMMON_DIR}/RawFile.cpp
    ${COMMON_DIR}/RawFile.hpp
    ${COMMON_DIR}/RawFileReadOnly.cpp
    ${COMMON_DIR}/RawFileReadOnly.hpp
    ${COMMON_DIR}/RawTypeNavigator.cpp
    ${COMMON_DIR}/RawTypeNavigator.hpp
    ${COMMON_DIR}/Software.hpp
    ${COMMON_DIR}/TemplateTypes.hpp
    ${COMMON_DIR}/Types.hpp)

set(SOURCES_FRONTENDQT
    ${FRONTENDQT_DIR}/AboutDialog.cpp
    ${FRONTENDQT_DIR}/AboutDialog.hpp
    ${FRONTENDQT_DIR}/AboutDialog.ui
    ${FRONTENDQT_DIR}/AddonsWidget.cpp
    ${FRONTENDQT_DIR}/AddonsWidget.hpp
    ${FRONTENDQT_DIR}/AddonsWidget.ui
    ${FRONTENDQT_DIR}/BossWidget.cpp
    ${FRONTENDQT_DIR}/BossWidget.hpp
    ${FRONTENDQT_DIR}/BossWidget.ui
    ${FRONTENDQT_DIR}/ChallengeWidget.cpp
    ${FRONTENDQT_DIR}/ChallengeWidget.hpp
    ${FRONTENDQT_DIR}/ChallengeWidget.ui
    ${FRONTENDQT_DIR}/EquipmentWidget.cpp
    ${FRONTENDQT_DIR}/EquipmentWidget.hpp
    ${FRONTENDQT_DIR}/EquipmentWidget.ui
    ${FRONTENDQT_DIR}/ExtractGameDialog.cpp
    ${FRONTENDQT_DIR}/ExtractGameDialog.hpp
    ${FRONTENDQT_DIR}/ExtractGameDialog.ui
    ${FRONTENDQT_DIR}/FixesWidget.cpp
    ${FRONTENDQT_DIR}/FixesWidget.hpp
    ${FRONTENDQT_DIR}/FixesWidget.ui
    ${FRONTENDQT_DIR}/ForestWidget.cpp
    ${FRONTENDQT_DIR}/ForestWidget.hpp
    ${FRONTENDQT_DIR}/ForestWidget.ui
    ${FRONTENDQT_DIR}/GuiPath.hpp
    ${FRONTENDQT_DIR}/GuiSettings.cpp
    ${FRONTENDQT_DIR}/GuiSettings.hpp
    ${FRONTENDQT_DIR}/HelpConsoleWidget.cpp
    ${FRONTENDQT_DIR}/HelpConsoleWidget.hpp
    ${FRONTENDQT_DIR}/HelpConsoleWidget.ui
    ${FRONTENDQT_DIR}/ImageAnimator.cpp
    ${FRONTENDQT_DIR}/ImageAnimator.hpp
    ${FRONTENDQT_DIR}/ImageAnimatorFactory.cpp
    ${FRONTENDQT_DIR}/ImageAnimatorFactory.hpp
    ${FRONTENDQT_DIR}/LevantWidget.cpp
    ${FRONTENDQT_DIR}/LevantWidget.hpp
    ${FRONTENDQT_DIR}/LevantWidget.ui
    ${FRONTENDQT_DIR}/Main.cpp
    ${FRONTENDQT_DIR}/MainWindow.cpp
    ${FRONTENDQT_DIR}/MainWindow.hpp
    ${FRONTENDQT_DIR}/MainWindow.ui
    ${FRONTENDQT_DIR}/MinionWidget.cpp
    ${FRONTENDQT_DIR}/MinionWidget.hpp
    ${FRONTENDQT_DIR}/MinionWidget.ui
    ${FRONTENDQT_DIR}/MiscWidget.cpp
    ${FRONTENDQT_DIR}/MiscWidget.hpp
    ${FRONTENDQT_DIR}/MiscWidget.ui
    ${FRONTENDQT_DIR}/QtUtil.cpp
    ${FRONTENDQT_DIR}/QtUtil.hpp
    ${FRONTENDQT_DIR}/RandomizerTabWidget.cpp
    ${FRONTENDQT_DIR}/RandomizerTabWidget.hpp
    ${FRONTENDQT_DIR}/RandomizerTabWidget.ui
    ${FRONTENDQT_DIR}/RandomizerUiManager.cpp
    ${FRONTENDQT_DIR}/RandomizerUiManager.hpp
    ${FRONTENDQT_DIR}/RandomizerWidget.cpp
    ${FRONTENDQT_DIR}/RandomizerWidget.hpp
    ${FRONTENDQT_DIR}/RandomizerWidgetSettings.cpp
    ${FRONTENDQT_DIR}/RandomizerWidgetSettings.hpp
    ${FRONTENDQT_DIR}/SaveGameDialog.cpp
    ${FRONTENDQT_DIR}/SaveGameDialog.hpp
    ${FRONTENDQT_DIR}/SaveGameDialog.ui
    ${FRONTENDQT_DIR}/ShopWidget.cpp
    ${FRONTENDQT_DIR}/ShopWidget.hpp
    ${FRONTENDQT_DIR}/ShopWidget.ui
    ${FRONTENDQT_DIR}/SpecialMagicDialog.cpp
    ${FRONTENDQT_DIR}/SpecialMagicDialog.hpp
    ${FRONTENDQT_DIR}/SpecialMagicDialog.ui
    ${FRONTENDQT_DIR}/TopInfoWidget.cpp
    ${FRONTENDQT_DIR}/TopInfoWidget.hpp
    ${FRONTENDQT_DIR}/TopInfoWidget.ui
    ${FRONTENDQT_DIR}/TreasureWidget.cpp
    ${FRONTENDQT_DIR}/TreasureWidget.hpp
    ${FRONTENDQT_DIR}/TreasureWidget.ui)

set(SOURCES_RESOURCEQT ${PROJECT_SOURCE_DIR}/src/FrontendQt/resources/resources.qrc)

if (WIN32)
    enable_language("RC")
    set(WIN32_RESOURCE ${FRONTENDQT_DIR}/resources/JCRandomizer.rc)
    add_executable(${PROJECT_NAME} WIN32 ${WIN32_RESOURCE})
else()
    add_executable(${PROJECT_NAME})
endif()

target_sources(${PROJECT_NAME} PRIVATE  
    ${SOURCES_BACKEND}
    ${SOURCES_BACKEND_RANDOMIZER}
    ${SOURCES_COMMON}
    ${SOURCES_FRONTENDQT}
    ${SOURCES_RESOURCEQT})

source_group(Backend FILES ${SOURCES_BACKEND})
source_group(Backend/Randomizer FILES ${SOURCES_BACKEND_RANDOMIZER})
source_group(Common FILES ${SOURCES_COMMON})
source_group(FrontendQt FILES ${SOURCES_FRONTENDQT})
source_group(FrontendQt/resources FILES ${SOURCES_RESOURCEQT})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/dep/Jade-Cocoon-Unpacker-Repacker/src
    ${PROJECT_SOURCE_DIR}/dep/json/include
    ${PROJECT_SOURCE_DIR}/dep/mkpsxiso/src)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:JCR_DEBUG>)

target_link_libraries(${PROJECT_NAME} 
    Qt6::Widgets 
    jade_cocoon_unpacker_repacker
    mkpsxiso
    dumpsxiso
)

if (MSVC)
    set(TARGETS iso_shared tinyxml2 mkpsxiso dumpsxiso)
    foreach(TARGET ${TARGETS})
        target_compile_options(${TARGET} PRIVATE $<$<CONFIG:Release>:/MD>)
        target_compile_options(${TARGET} PRIVATE $<$<CONFIG:Debug>:/MDd>)
    endforeach()
endif()