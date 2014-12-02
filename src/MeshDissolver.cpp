#include "MeshDissolver.h"

#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>
#include <maya/MItMeshPolygon.h>
#include <iostream>
#include <string>

#include <time.h>
 
void* MeshDissolver::creator() { return new MeshDissolver; }
 
MStatus MeshDissolver::doIt(const MArgList& argList) {
	clock_t t = clock();
    MVector vector( 1.0, 0.0, 0.0 );
    MStatus stat;

    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
	MItSelectionList iter(selection, MFn::kInvalid, &stat);

	//MGlobal::executeCommand("move -r 0.0 30.0 0.0");

    if (MS::kSuccess == stat) {
        MDagPath mdagPath; 
        MObject mComponent;
		MVector vector = MVector(0.0, 30.0, 0.0);

        // Loop through each mesh
        for (; !iter.isDone(); iter.next()) {
            iter.getDagPath(mdagPath, mComponent);

			// Translate each mesh by vector
			MItMeshVertex vertexIter(mdagPath);
			for (; !vertexIter.isDone(); vertexIter.next()) {
				vertexIter.translateBy(vector);
			}
			
			// Translate each face in a mesh
			/*MItMeshPolygon faceIter(mdagPath);
			if (MS::kSuccess == stat) { 
				for (; !faceIter.isDone(); faceIter.next()) {
					MPointArray pointArray;
					faceIter.getPoints(pointArray);
					for (unsigned int i = 0; i < pointArray.length(); i++) {
						pointArray[i].x += vector.x;
						pointArray[i].y += vector.y;
						pointArray[i].z += vector.z;
					}
					faceIter.setPoints(pointArray);

					faceIter.updateSurface();
				}
			}*/
        }
    } 
	t = clock() - t;
	cout << ((float)t) / CLOCKS_PER_SEC << endl;
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