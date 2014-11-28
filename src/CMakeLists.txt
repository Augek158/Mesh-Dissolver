if (APPLE)

	# Path to the maya includes.
	set(MAYA_INCLUDE_PATH $ENV{MAYA_INCLUDES})

	# Path to the maya libraries.
	set(MAYA_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})

	# Set library extension
	set(LIB_EXT dylib)

	# Set binary extension
	set(BIN_EXT bundle)

	# Set up the directories.
	include_directories(${MeshDissolver_SOURCE_DIR}/include : ${MAYA_INCLUDE_PATH})
	link_directories(${MAYA_LIBRARY_PATH})

	# Compiler flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DREQUIRE_IOSTREAM -include '${MAYA_INCLUDE_PATH}/maya/OpenMayaMac.h' ")

endif (APPLE)
	# Path to the maya includes.
	set(MAYA_INCLUDE_PATH "C:/Program Files/Autodesk/Maya2015/include")

	# Path to the maya libraries.
	set(MAYA_LIBRARY_PATH "C:/Program Files/Autodesk/Maya2015/lib")

	# Set library extension
	set(LIB_EXT lib)
	
	# Set binary extension
	set(BIN_EXT mll)

	# Set up the directories.
	include_directories(${MeshDissolver_SOURCE_DIR}/include : ${MAYA_INCLUDE_PATH})
	link_directories(${MAYA_LIBRARY_PATH})
	
	set_target_properties(
	HelloWorld PROPERTIES SUFFIX ".mll" LINK_FLAGS 
	"/export:initializePlugin /export:uninitializePlugin"
	)
		
endif (WIN32)

# The libraries for linking.
set(LIBS OpenMaya.${LIB_EXT} Foundation.${LIB_EXT})	

add_executable(HelloWorld.${BIN_EXT} HelloWorld.cpp)
target_link_libraries(HelloWorld.${BIN_EXT} ${LIBS})

