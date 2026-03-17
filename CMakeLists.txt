cmake_minimum_required(VERSION 3.22)
project(onodrim LANGUAGES CXX)

option(ONONDRIM_BUILD_TEST          "Build tests using onodrim"                     OFF )
option(ONONDRIM_BUILD_DOCUMENTATION "Build documentation using Doxygen for Onodrim" OFF )

##======================================================================================================================
include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

##======================================================================================================================
## Project setup via copacabana
##======================================================================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake ${COPACABANA_SOURCE_DIR}/copacabana/cmake)
include(${COPACABANA_SOURCE_DIR}/copacabana/cmake/copacabana.cmake)
copa_project_version(MAJOR 1 MINOR 0 PATCH 0)

##======================================================================================================================
## Summary Display
##======================================================================================================================
if(NOT ONODRIM_QUIET)
  if(CMAKE_BUILD_TYPE)
    message(STATUS "[${PROJECT_NAME}] - Building in ${CMAKE_BUILD_TYPE} mode")
  endif()

  message(STATUS "[${PROJECT_NAME}] - Unit tests              : ${ONONDRIM_BUILD_TEST} (via ONONDRIM_BUILD_TEST)")
  message(STATUS "[${PROJECT_NAME}] - Doxygen                 : ${ONONDRIM_BUILD_DOCUMENTATION} (via ONONDRIM_BUILD_DOCUMENTATION)")
  set(QUIET_OPTION "")
else()
  set(QUIET_OPTION "QUIET")
endif()

##======================================================================================================================
## Pre-commit hooks setup targets
##======================================================================================================================
copa_setup_precommit_hooks(${QUIET_OPTION})
