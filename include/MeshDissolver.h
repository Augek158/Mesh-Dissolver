#ifndef MESH_DISSOLVER_H
#define MESH_DISSOLVER_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MDagPath.h>

typedef struct FaceData {
	int numVertices;
	int numPolygons;
	MFloatPointArray vertexArray;
	MIntArray polygonCounts;
	MIntArray polygonConnects;
} FaceData;

class MeshDissolver : public MPxCommand {
 public:
   MeshDissolver() {};
   ~MeshDissolver();
   virtual MStatus doIt(const MArgList& argList);
   virtual MStatus redoIt();
   static void* creator();
 private:
 	FaceData* faceData;
 	bool checkStatus(const MStatus& stat);
 	bool collectFaceData(const MDagPath& mdagPath, FaceData* faceData); 
	void translateMesh(MVector vector, MDagPath mdagPath);
	void translateFace(MVector vector, MDagPath mdagPath);
};

#endif // MESH_DISSOLVER_H