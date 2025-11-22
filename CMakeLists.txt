cmake_minimum_required(VERSION 3.14.0)

project(Rubidium
    VERSION 0.4.0
    DESCRIPTION "Rubidium VST 3 Plug-in"
)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "./vst3sdk")
option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF)
add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(Rubidium
    source/version.h
    source/rbd_cids.h
    source/rbd_processor.h
    source/rbd_processor.cpp
    source/rbd_controller.h
    source/rbd_controller.cpp
    source/rbd_entry.cpp
)

target_link_libraries(Rubidium
    PRIVATE
        sdk
)

smtg_target_configure_version_file(Rubidium)

if(SMTG_MAC)
    smtg_target_set_bundle(Rubidium
        BUNDLE_IDENTIFIER com.venusbrock.vst3.rubidium
        COMPANY_NAME "Venus Brock"
    )
    smtg_target_set_debug_executable(Rubidium
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(Rubidium PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Rubidium)

        smtg_target_set_debug_executable(Rubidium
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)

target_link_libraries(Rubidium
    PRIVATE
        vstgui_support
)

smtg_target_add_plugin_resources(Rubidium
    RESOURCES
        "resource/rbd_editor.uidesc"
        "resource/rbd_bg.png"
)
