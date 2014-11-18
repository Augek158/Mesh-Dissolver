IF(APPLE)

include_directories(${MeshDissolver_SOURCE_DIR}/include : /Applications/Autodesk/maya2015/devkit/include)

set(MAYA_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DREQUIRE_IOSTREAM -include '/Applications/Autodesk/maya2015/devkit/include/maya/OpenMayaMac.h' ")

LINK_DIRECTORIES(${MAYA_LIBRARY_PATH})
set(LIBS libOpenMaya.dylib libFoundation.dylib)

ENDIF (APPLE)

add_executable(HelloWorld.bundle HelloWorld.cpp)
target_link_libraries(HelloWorld.bundle ${LIBS})

