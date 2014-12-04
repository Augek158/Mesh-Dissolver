#include "MeshDissolver.h"

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
        MObject mObject, mTransform;

        // Get DagPath of the first object.
        selection.getDagPath(0, mdagPath);

        // Get transform of the DagPath.
        mTransform = mdagPath.transform(&stat);
        if (!checkStatus(stat)) return MS::kFailure;

        // Allocate on heap since it is a big struct.
        faceData = new FaceData;

        // TODO: Curved 3D spaces requires the user to do an "Average normal"
        // in Maya to interpolate the normals. A MEL script could probably take care of this.
        if (!collectFaceData(mdagPath, faceData)) return MS::kFailure;

        // Use MEL to delete the original shape.
        mdagPath.extendToShape();
        MString deleteOriginalStr = "delete " + mdagPath.fullPathName();
        MGlobal::executeCommand(deleteOriginalStr);

        MFnMesh surfFn;
        surfFn.create(faceData->numVertices, faceData->numPolygons, faceData->vertexArray,
                      faceData->polygonCounts, faceData->polygonConnects, mTransform);
        /*
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
        */
        
    } 

	// How long did the calculation take?
	t = clock() - t;
	cout << ((float)t) / CLOCKS_PER_SEC << endl;
    
	return MS::kSuccess;
}

/// Takes an MDagPath and a FaceData* as parameter. The function uses the mesh at the
/// MDagPath to populate the data in FaceData*.
/// The function handles meshes with varying number of vertices per face, e.g. spheres.
/// 
/// Returns MS::kSuccess if successfully executed. 
bool MeshDissolver::collectFaceData(const MDagPath& mdagPath, FaceData* faceData) {

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
        for (int i = 0; i < pointArray.length(); i++) {
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
bool MeshDissolver::checkStatus (const MStatus& stat) { 
    if (stat != MS::kSuccess) {
        MGlobal::displayError(stat.errorString());
        return false;
    }
    return true;
}

MStatus MeshDissolver::redoIt (){

    // Do actual work here with the data from local class.
    return MS::kSuccess;
}

MeshDissolver::~MeshDissolver() {
    delete faceData;
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
