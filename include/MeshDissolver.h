#ifndef MESH_DISSOLVER_H
#define MESH_DISSOLVER_H
 
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MObject.h>
 
class MeshDissolver : public MPxCommand {
 public:
   MeshDissolver() {};
   virtual MStatus doIt(const MArgList& argList);
   virtual MStatus redoIt();
   static void* creator();
};
#endif