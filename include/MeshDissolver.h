#ifndef MESH_DISSOLVER_H
#define MESH_DISSOLVER_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MDagPath.h>

class MeshDissolver : public MPxCommand {
 public:
   MeshDissolver() {};
   virtual MStatus doIt(const MArgList& argList);
   virtual MStatus redoIt();
   static void* creator();
private:
	void translateMesh(MVector vector, MDagPath mdagPath);
	void translateFace(MVector vector, MDagPath mdagPath);
};

#endif // MESH_DISSOLVER_H