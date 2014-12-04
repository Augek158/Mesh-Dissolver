#include "MeshDissolver.h"

#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MGlobal.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MItKeyframe.h>

#include <iostream>
#include <string>
#include <time.h>
 
void* MeshDissolver::creator() { return new MeshDissolver; }
 
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
		MVector vector = MVector(0.0, 10.0, 0.0);

		MObject curve;
		MFnAnimCurve fnCurve(curve);
		MItKeyframe kIt(curve);

		// Loop through each mesh
		for (; !iter.isDone(); iter.next()) {
			iter.getDagPath(mdagPath, mComponent);

			while(!kIt.isDone()) {
				cout << "hej" << endl;
			}

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
void MeshDissolver::translateMesh(MVector vector, MDagPath mdagPath) {
	MItMeshVertex vertexIter(mdagPath);
	int vertIndices = 0;

	for (; !vertexIter.isDone(); vertexIter.next()) {
		// Set a keyframe at position 0 for the specified face
		MString command1;
		command1 += "setKeyframe -t 0 ";
		command1 += mdagPath.fullPathName();
		command1 += ".vtx[";
		command1 += vertIndices;
		command1 += "]";
		MGlobal::executeCommand(command1);

		vertexIter.translateBy(vector);

		// Set a keyframe at position 300 for the specified face
		MString command2;
		command2 += "setKeyframe -t 200 ";
		command2 += mdagPath.fullPathName();
		command2 += ".vtx[";
		command2 += vertIndices;
		command2 += "]";
		MGlobal::executeCommand(command2);

		vertIndices++;
	}
}

/// Translate each face with specific vector
void MeshDissolver::translateFace(MVector vector, MDagPath mdagPath) {
	MItMeshPolygon faceIter(mdagPath);
	int faceIndices = 0;
	MString command;

	for (; !faceIter.isDone(); faceIter.next()) {
		// Set a keyframe at position 0 for the specified face
		command = "";
		command += "setKeyframe -t 0";
		command += mdagPath.fullPathName();
		command += ".f[";
		command += faceIndices;
		command += "]";
		MGlobal::executeCommand(command);

		// Translate the face with the specified vector
		MPointArray pointArray;
		faceIter.getPoints(pointArray);
		for (unsigned int i = 0; i < pointArray.length(); i++) {
			pointArray[i].x += vector.x;
			pointArray[i].y += vector.y;
			pointArray[i].z += vector.z;
		}
		faceIter.setPoints(pointArray);
		faceIter.updateSurface();
		
		// Set a keyframe at position 300 for the specified face
		command = "";
		command += "setKeyframe -t 200";
		command += mdagPath.fullPathName();
		command += ".f[";
		command += faceIndices;
		command += "]";
		MGlobal::executeCommand(command);
		
		faceIndices++;
	}
}
