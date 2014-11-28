#ifndef HELLOWORLD_H
#define HELLOWORLD_H
 
#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>

#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MObject.h>
 
class HelloWorld : public MPxCommand {
 public:
   HelloWorld() {};
   virtual MStatus doIt(const MArgList& argList);
   virtual MStatus redoIt();
   static void* creator();
};
#endif