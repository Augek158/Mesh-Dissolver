#ifndef EXTRACT_FACE_COMMAND_H
#define EXTRACT_FACE_COMMAND_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MDagPath.h>

/// Struct to hold information about the faces of a mesh.
typedef struct FaceData {
	int numVertices;
	int numPolygons;
	MFloatPointArray vertexArray;
	MIntArray polygonCounts;
	MIntArray polygonConnects;
} FaceData;

class ExtractFaceCommand : public MPxCommand {
  public:
    ExtractFaceCommand();
    ~ExtractFaceCommand(){};
    virtual MStatus doIt(const MArgList& argList);
    static void* creator();
   
  private:

 	/// Takes an MDagPath and a FaceData* as parameter. The function uses the mesh at the
	/// MDagPath to populate the data in FaceData*.
	/// The function handles meshes with varying number of vertices per face, e.g. spheres.
	/// 
	/// Returns MS::kSuccess if successfully executed. 
 	bool collectFaceData(const MDagPath& mdagPath, FaceData* faceData); 

 	/// Translate each mesh with specific vector.
	void translateMesh(MVector vector, MDagPath mdagPath);

	/// Translate each face with specific vector.
	void translateFace(MVector vector, MDagPath mdagPath);

	/// Utility function to check if a status is ok.
 	bool checkStatus(const MStatus& stat);
};

#endif // EXTRACT_FACE_COMMAND_H