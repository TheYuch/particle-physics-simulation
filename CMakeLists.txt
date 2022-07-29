#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.21)
project(TEST_PROJECT)


#----------------------------------------------------------------------------
# Geant4
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})


#----------------------------------------------------------------------------
# ROOT
#
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include("${ROOT_DIR}/RootMacros.cmake")


#----------------------------------------------------------------------------
# Include directories
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/include/detectors
                    ${PROJECT_SOURCE_DIR}/include/detectors/hitdata
                    ${PROJECT_SOURCE_DIR}/include/rootdata
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/detectors/*.cc)# ${PROJECT_SOURCE_DIR}/src/detectors/hitdata/*.cc)
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
# Add and link the executable
#
add_executable(test test.cc ${SOURCES} ${HEADERS})
target_link_libraries(test ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} RootDict) # new - added root


#----------------------------------------------------------------------------
# Installation
#
install(TARGETS test DESTINATION bin)