#define NT_PLUGIN
#define REQUIRE_IOSTREAM

#include "HelloWorld.h"

void* HelloWorld::creator() { return new HelloWorld; }
 
MStatus HelloWorld::doIt(const MArgList& argList) {

    MVector vector( 1.0, 0.0, 0.0 );
    MStatus stat;
    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
    MItSelectionList iter(selection, MFn::kInvalid, &stat );

    if (MS::kSuccess == stat) {
        MDagPath mdagPath; 
        MObject mComponent; 
        MGlobal::displayInfo("Created Object.");
        
        for (; !iter.isDone(); iter.next()) {
            iter.getDagPath(mdagPath, mComponent);
        }
    }

    redoIt();

    return MS::kSuccess;
}

MStatus HelloWorld::redoIt (){

    // Do actual work here with the data from local class.
    
    return MS::kSuccess;
}
 
MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "August Ek & Ramin Assadi", "0.1", "Any");
    MStatus status = plugin.registerCommand("helloWorld", HelloWorld::creator);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}
 
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);
    MStatus status = plugin.deregisterCommand("helloWorld");
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}

int main(int argc, char** argv) {

    if (argc != 2) return 0;

    return 0;

}