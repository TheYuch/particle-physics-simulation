#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.21)
project(TEST_PROJECT)


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Include ROOT
#
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include("${ROOT_DIR}/RootMacros.cmake")


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/detectors/*.cc ${PROJECT_SOURCE_DIR}/src/detectors/hitdata/*.cc)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/detectors/*.hh ${PROJECT_SOURCE_DIR}/include/detectors/hitdata/*.hh)

file(GLOB ROOT_DATA_SOURCES ${PROJECT_SOURCE_DIR}/src/rootdata/*.cc)
file(GLOB ROOT_DATA_HEADERS ${PROJECT_SOURCE_DIR}/include/rootdata/*.hh)


#----------------------------------------------------------------------------
# ROOT dictionary compilation
#
add_library(RootDict SHARED ${ROOT_DATA_SOURCES})
target_link_libraries(RootDict ${ROOT_LIBRARIES})

root_generate_dictionary(genRootDict ${ROOT_DATA_HEADERS}
      MODULE RootDict
      LINKDEF ${PROJECT_SOURCE_DIR}/include/rootdata/LinkDef.h
      )


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(test test.cc ${SOURCES} ${HEADERS})
target_link_libraries(test ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} RootDict) # new - added root


#----------------------------------------------------------------------------
# Installation
#
install(TARGETS test DESTINATION bin)