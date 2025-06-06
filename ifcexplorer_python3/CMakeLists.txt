cmake_minimum_required(VERSION 3.25)

project(ifcexplorer_python3)

# copy scripts
set(IFCEXPLORER_PYTHON3_COPY_SCRIPTS TRUE CACHE BOOL "copy scripts to output directory")

if (IFCEXPLORER_PYTHON3_COPY_SCRIPTS)
    file(GLOB_RECURSE SCRIPT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.py ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.pyi)
    add_custom_target(ifcexplorer_python3_scripts
            DEPENDS ${SCRIPT_FILES} COMMAND_EXPAND_LISTS
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/scripts $<TARGET_FILE_DIR:ifcexplorer_python3>/scripts
            )
endif()

# build plugin
find_package(wxWidgets CONFIG REQUIRED)
find_package(Python3 COMPONENTS Development Interpreter REQUIRED)

add_library(ifcexplorer_python3 SHARED
        src/PluginPython3.cpp
        src/PluginPython3.hpp
        src/ModuleDocument.cpp
        src/ModuleDocument.hpp
        src/IfcDBState.hpp
        src/PythonAction.cpp
        src/PythonAction.hpp
        src/Interpreter.cpp
        src/Interpreter.hpp
        src/PluginState.hpp
        src/PluginModule.hpp
        src/PluginModule.cpp
        src/Population.hpp
        src/IfcDBModule.cpp
        src/IfcDBModule.hpp
        src/InterpreterError.hpp
        src/InterpreterError.cpp
        src/Entity.hpp
        src/Entity.cpp
        src/Geometry.hpp
        src/Geometry.cpp
        src/Relation.hpp
        src/Relation.cpp
        src/EntityType.hpp
        src/EntityType.cpp
        src/Property.cpp
        src/VenvAction.cpp
        src/VenvAction.hpp
        src/StandaloneScript.cpp
        src/PythonPluginManifest.hpp
        src/fs.hpp
        src/ManifestAction.cpp
        src/ManifestAction.hpp
        src/Material.cpp
        src/venvDlg.h
        src/venvDlg.cpp
        src/venvDlgImpl.h
        src/venvDlgImpl.cpp
        src/Face.hpp
        src/Face.cpp
        src/Polyline.hpp
        src/Polyline.cpp)

if( CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(ifcexplorer_python3 PRIVATE ifcexplorer_plugin_d stdutils_d ifcdb_d gps_d Rpcrt4 glu32)
elseif( CMAKE_BUILD_TYPE STREQUAL "Release")
  target_link_libraries(ifcexplorer_python3 PRIVATE ifcexplorer_plugin stdutils ifcdb gps Rpcrt4 glu32)
else()
    MESSAGE( STATUS "CMAKE_BUILD_TYPE not set yet ${CMAKE_BUILD_TYPE}" )
endif()

target_include_directories(ifcexplorer_python3 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_directories(ifcexplorer_python3 PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(ifcexplorer_python3 PRIVATE Python3::Python)
target_link_libraries(ifcexplorer_python3 PRIVATE wx::core wx::base)
target_compile_features(ifcexplorer_python3 PRIVATE cxx_std_17)
add_dependencies(ifcexplorer_python3 ifcexplorer_python3_scripts)

set(IFCEXPLORER_PYTHON3_TESTS_ENABLED FALSE CACHE BOOL "ifcexplorer_python3 tests enabled")

if (IFCEXPLORER_PYTHON3_TESTS_ENABLED)
    find_package(Catch2 CONFIG REQUIRED)

    add_executable(test_ifcexplorer_python3 src/tests/test_plugin.cpp)
    target_link_libraries(test_ifcexplorer_python3 PRIVATE ifcexplorer_plugin Catch2::Catch2 Catch2::Catch2WithMain)

    add_dependencies(test_ifcexplorer_python3 ifcexplorer_python3)

    add_test(NAME test_ifcexplorer_python3 COMMAND test_ifcexplorer_python3)
endif(IFCEXPLORER_PYTHON3_TESTS_ENABLED)

get_filename_component(EMBED_PYTHON_HOME ${Python3_EXECUTABLE} DIRECTORY)

add_custom_command( TARGET ifcexplorer_python3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${EMBED_PYTHON_HOME}
        $<TARGET_FILE_DIR:ifcexplorer_python3>/distrib
        COMMAND_EXPAND_LISTS)