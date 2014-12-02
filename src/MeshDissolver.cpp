#include "MeshDissolver.h"

#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MGlobal.h>

#include <iostream>
#include <string>
#include <time.h>
 
void* MeshDissolver::creator() { return new MeshDissolver; }
void translateMesh(MVector vector, MDagPath mdagPath);
void translateFace(MVector vector, MDagPath mdagPath);
 
MStatus MeshDissolver::doIt(const MArgList& argList) {
	
	clock_t t = clock();
    MStatus stat;

	// Get current selection
    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
	MItSelectionList iter(selection, MFn::kInvalid, &stat);
    
    if (MS::kSuccess == stat) {
        MDagPath mdagPath; 
        MObject mComponent;
		MVector vector = MVector(0.0, 30.0, 0.0);

        // Loop through each mesh
        for (; !iter.isDone(); iter.next()) {
            iter.getDagPath(mdagPath, mComponent);

			translateMesh(vector, mdagPath);
			//translateFace(vector, mdagPath);
        }
    } 

	// How long did the calculation take?
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

/// Translate each mesh with specific vector
void translateMesh(MVector vector, MDagPath mdagPath) {
	MItMeshVertex vertexIter(mdagPath);
	for (; !vertexIter.isDone(); vertexIter.next()) {
		vertexIter.translateBy(vector);
	}
}

/// Translate each face with specific vector
void translateFace(MVector vector, MDagPath mdagPath) {
	MItMeshPolygon faceIter(mdagPath);
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
}