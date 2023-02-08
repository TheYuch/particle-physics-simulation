#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.21)

set(name xintil)
project(${name})


#----------------------------------------------------------------------------
# Locate and include packages
#

# Geant4
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

# ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include("${ROOT_DIR}/RootMacros.cmake")


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/include/actions
                    ${PROJECT_SOURCE_DIR}/include/detectors
                    ${PROJECT_SOURCE_DIR}/include/hitdata
                    ${PROJECT_SOURCE_DIR}/include/rootdata
                    ${PROJECT_SOURCE_DIR}/include/utils
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc
                  ${PROJECT_SOURCE_DIR}/src/actions/*.cc
                  ${PROJECT_SOURCE_DIR}/src/hitdata/*.cc
                  ${PROJECT_SOURCE_DIR}/src/utils/*.cc)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.hh
                  ${PROJECT_SOURCE_DIR}/include/actions/*.hh
                  ${PROJECT_SOURCE_DIR}/include/detectors/*.hh
                  ${PROJECT_SOURCE_DIR}/include/hitdata/*.hh
                  ${PROJECT_SOURCE_DIR}/include/utils/*.hh)

file(GLOB ROOT_DATA_SOURCES ${PROJECT_SOURCE_DIR}/src/rootdata/*.cc)
file(GLOB ROOT_DATA_HEADERS ${PROJECT_SOURCE_DIR}/include/rootdata/*.hh)

file(GLOB macros ${PROJECT_SOURCE_DIR}/macros/*.mac)


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
add_executable(${name} ${name}.cc ${SOURCES} ${HEADERS})
target_link_libraries(${name} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} RootDict) # TODO: separate lines as in PIONEER git repo? make root independent of geant4


#----------------------------------------------------------------------------
# Installation
#
install(TARGETS ${name} DESTINATION bin)
install(FILES ${macros} DESTINATION bin/macros)

install(TARGETS RootDict DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libRootDict_rdict.pcm ${CMAKE_CURRENT_BINARY_DIR}/libRootDict.rootmap DESTINATION lib)