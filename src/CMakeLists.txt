if (APPLE)

	# Path to the maya includes.
	set(MAYA_INCLUDE_PATH $ENV{MAYA_INCLUDES})

	# Path to the maya libraries.
	set(MAYA_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})

	# Set up the directories.
	include_directories(${MeshDissolver_SOURCE_DIR}/include : ${MAYA_INCLUDE_PATH})
	link_directories(${MAYA_LIBRARY_PATH})

	# Compiler flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DREQUIRE_IOSTREAM -include '${MAYA_INCLUDE_PATH}/maya/OpenMayaMac.h' ")

	# The libraries for linking.
	set(LIBS libOpenMaya.dylib libFoundation.dylib)

endif (APPLE)

add_executable(HelloWorld.bundle HelloWorld.cpp)
target_link_libraries(HelloWorld.bundle ${LIBS})

