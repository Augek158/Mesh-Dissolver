#ifndef MESH_DISSOLVER_H
#define MESH_DISSOLVER_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>

typedef struct Face {
	int numVertices;
	int numPolygons;
	MFloatPointArray vertexArray;
	MIntArray polygonCounts;
	MIntArray polygonConnects;
	MFloatArray uArray;
	MFloatArray vArray;
} Face;

class MeshDissolver : public MPxCommand {
 public:
   MeshDissolver() {};
   virtual MStatus doIt(const MArgList& argList);
   virtual MStatus redoIt();
   static void* creator();

 private:
 	bool checkStatus(const MStatus& stat);

};

#endif // MESH_DISSOLVER_H