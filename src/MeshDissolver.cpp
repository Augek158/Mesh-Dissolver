#include "MeshDissolver.h"
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
 
void* MeshDissolver::creator() { return new MeshDissolver; }
 
MStatus MeshDissolver::doIt(const MArgList& argList) {

    MStatus stat; 
    MSelectionList selection;
    char msg[100];

    MGlobal::getActiveSelectionList(selection);
    
    if (MS::kSuccess == stat) {
        MDagPath mdagPath;
        MObject face;
        
        // Get DagPath of the first object.
        selection.getDagPath(0, mdagPath);

        // Create iterator.
        MItMeshPolygon faceIter(mdagPath);

            // Iterates through all faces of the polygon.
            for(; !faceIter.isDone(); faceIter.next()) {
                face = faceIter.currentItem();

                // Replace current selection with 'face'.
                MGlobal::select(mdagPath, face, MGlobal::kReplaceList);

                // Execute MEL command to extract face.
                MGlobal::executeCommand("ExtractFace");
            }     
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