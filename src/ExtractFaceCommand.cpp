#include "ExtractFaceCommand.h"

#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>

#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MVector.h>
#include <maya/MGlobal.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MItKeyframe.h>

#include <time.h>

ExtractFaceCommand::ExtractFaceCommand() { 

}
 
void* ExtractFaceCommand::creator() { return new ExtractFaceCommand; }
 
MStatus ExtractFaceCommand::doIt(const MArgList& argList) {
	
    MStatus stat;
    FaceData faceData = {};

	// Get current selection
    MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
	MItSelectionList iter(selection, MFn::kInvalid, &stat);
    
    if (MS::kSuccess == stat) {
        MDagPath mdagPath;
        MObject mObject, mTransform;

        // Get DagPath of the first object.
        selection.getDagPath(0, mdagPath);

        // Get transform of the DagPath.
        mTransform = mdagPath.transform(&stat);
        if (!checkStatus(stat)) return MS::kFailure;

        // TODO: Curved 3D spaces requires the user to do an "Average normal"
        // in Maya to interpolate the normals. A MEL script could probably take care of this.
        if (!collectFaceData(mdagPath, &faceData)) return MS::kFailure;

        // Use MEL to delete the original shape.
        mdagPath.extendToShape();
        MString deleteOriginalStr = "delete " + mdagPath.fullPathName();
        MGlobal::executeCommand(deleteOriginalStr);

        MFnMesh surfFn;
        surfFn.create(faceData.numVertices, faceData.numPolygons, faceData.vertexArray,
                      faceData.polygonCounts, faceData.polygonConnects, mTransform);
    } 
	return MS::kSuccess;
}

bool ExtractFaceCommand::collectFaceData(const MDagPath& mdagPath, FaceData* faceData) {

    MStatus stat;
    MItMeshPolygon faceIter(mdagPath, MObject::kNullObj, &stat);
    if (!checkStatus(stat)) return false;

    int indexOffset = 0;
    int numVertices = 0;
    int numPolygons = 0;
    float fPoint[4] = {0.0, 0.0, 0.0, 0.0};
    MPointArray pointArray;

    // Loop through each face.
    for (; !faceIter.isDone(); faceIter.next()) {

        // Fetch vertices.
        faceIter.getPoints(pointArray);

        // Cast MPoint to MFloatPoint and append to array of vertices.
        for (unsigned int i = 0; i < pointArray.length(); i++) {
            pointArray[i].get(fPoint);
            faceData->vertexArray.append(fPoint);
            faceData->polygonConnects.append(indexOffset + i);
        }

        // Set number of vertices for the face.
        faceData->polygonCounts.append(pointArray.length());

        numVertices += faceIter.polygonVertexCount();
        numPolygons += 1;
        indexOffset += pointArray.length();
    }

    faceData->numVertices = numVertices;
    faceData->numPolygons = numPolygons;

    return true;
}

void ExtractFaceCommand::translateMesh(MVector vector, MDagPath mdagPath) {
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

void ExtractFaceCommand::translateFace(MVector vector, MDagPath mdagPath) {
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

bool ExtractFaceCommand::checkStatus (const MStatus& stat) { 
    if (stat != MS::kSuccess) {
        MGlobal::displayError(stat.errorString());
        return false;
    }
    return true;
}
