#include "MeshDissolver.h"

#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>
 
void* MeshDissolver::creator() { return new MeshDissolver; }
 
MStatus MeshDissolver::doIt(const MArgList& argList) {

    MVector vector( 1.0, 0.0, 0.0 );
    MStatus stat;
    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
    MItSelectionList iter(selection, MFn::kInvalid, &stat );

    if (MS::kSuccess == stat) {
        MDagPath mdagPath; 
        MObject mComponent; 
        
        for (; !iter.isDone(); iter.next()) {
            iter.getDagPath(mdagPath, mComponent);

            MItMeshVertex vtxFn( mdagPath, mComponent, &stat );
            if ( MS::kSuccess == stat ) { // Vertex iterator successfully initalized.
                for ( ; !vtxFn.isDone(); vtxFn.next() ) {
                     if ( MS::kFailure == vtxFn.translateBy( vector ) ) {
                         cerr << "Error setting Vertex\n";
                     }
                 }
                 vtxFn.updateSurface();
            } 
        }
    } else {
        cerr << "Failed to create selection iterator." << endl;
    }




    return MS::kSuccess;
}

MStatus MeshDissolver::redoIt (){

    // Do actual work here with the data from local class.
    
    return MS::kSuccess;
}
 
MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "August Ek & Ramin Assadi", "0.1", "Any");
    MStatus status = plugin.registerCommand("dissolve", MeshDissolver::creator);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}
 
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);
    MStatus status = plugin.deregisterCommand("dissolve");
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}

int main(int argc, char** argv) {

    if (argc != 2) return 0;

    return 0;

}