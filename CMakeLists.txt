cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME "FluxLab")
set(VERSION 0.1.0)

set(PRODUCT_NAME "Flux Lab")
set(COMPANY_NAME "Nierika")
set(BUNDLE_ID "com.nierika.fluxlab")
set(MANUFACTURER_CODE "Nrka")
set(PLUGIN_CODE "Flab")
set(FORMATS Standalone AU VST3 AUv3)

set(CMAKE_CXX_STANDARD 20)
set(JUCE_VERSION 8.0.7)
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libs")

project(${PROJECT_NAME} VERSION ${VERSION})

set(MAJOR_VERSION ${CMAKE_PROJECT_VERSION_MAJOR})

if (${VERSION} VERSION_GREATER_EQUAL "2.0.0")
    set(DISPLAY_NAME "${PRODUCT_NAME} v${MAJOR_VERSION}")
else ()
    set(DISPLAY_NAME "${PRODUCT_NAME}")
endif ()

include(${CMAKE_DIR}/cpm.cmake)

cpmaddpackage(
        NAME JUCE
        GIT_TAG ${JUCE_VERSION}
        VERSION ${JUCE_VERSION}
        GITHUB_REPOSITORY juce-framework/JUCE
        SOURCE_DIR ${LIB_DIR}/juce
)

include(${CMAKE_DIR}/warnings.cmake)
include(${CMAKE_DIR}/utils.cmake)

juce_add_module("${LIB_DIR}/nierika_dsp" ALIAS_NAMESPACE nierika)

juce_add_plugin(
        ${PROJECT_NAME}
        #ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
        COMPANY_NAME "${COMPANY_NAME}"
        BUNDLE_ID "${BUNDLE_ID}"
        PLUGIN_MANUFACTURER_CODE "${MANUFACTURER_CODE}"
        PLUGIN_CODE "${PLUGIN_CODE}"
        FORMATS ${FORMATS}
        PRODUCT_NAME ${DISPLAY_NAME}

        COPY_PLUGIN_AFTER_BUILD TRUE
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HeaderFiles CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles} )
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
target_link_libraries(${PROJECT_NAME} PRIVATE nierika::nierika_dsp)

target_compile_definitions(${PROJECT_NAME} PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)

# Treat warnings as errors
# set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_OPTIONS "${PROJECT_WARNINGS_CXX}")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)