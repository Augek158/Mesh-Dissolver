#ifndef MESH_DISSOLVER_H
#define MESH_DISSOLVER_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>

class MeshDissolver : public MPxCommand {
 public:
   MeshDissolver() {};
   virtual MStatus doIt(const MArgList& argList);
   virtual MStatus redoIt();
   static void* creator();
};

#endif // MESH_DISSOLVER_H