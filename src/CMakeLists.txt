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
	
	add_executable(MeshDissolver.${BIN_EXT} main.cpp MeshDissolver.cpp)
	
	# The libraries for linking.
	set(LIBS libOpenMaya.${LIB_EXT} libFoundation.${LIB_EXT})

	# Add header file
	target_link_libraries(MeshDissolver.${BIN_EXT} ${LIBS})

endif (APPLE)

if (WIN32)
	set(SOURCES
   	"C:/Users/Ramin Assadi/Desktop/Mesh-Dissolver/src/main.cpp"
	"C:/Users/Ramin Assadi/Desktop/Mesh-Dissolver/src/MeshDissolver.cpp"
	)

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
	
	add_definitions(
		"/D \"NT_PLUGIN\""
		"/D \"REQUIRE_IOSTREAM\""
	)	

	add_library( MeshDissolver SHARED ${SOURCES} )

	set_target_properties( MeshDissolver PROPERTIES SUFFIX ".mll" LINK_FLAGS 
				"/dll /export:initializePlugin /export:uninitializePlugin" )	
	
	# The libraries for linking.
	set(LIBS OpenMaya.${LIB_EXT} Foundation.${LIB_EXT} OpenMayaAnim.${LIB_EXT} OpenMayaUI.${LIB_EXT})
	
	# Add header file
	target_link_libraries(MeshDissolver ${LIBS})
endif (WIN32)

	



