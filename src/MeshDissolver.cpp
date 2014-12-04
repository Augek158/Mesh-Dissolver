#include "MeshDissolver.h"

// Maya includes
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MDagModifier.h>
#include <maya/MGlobal.h>

// Maya function Sets
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>

 
void* MeshDissolver::creator() { return new MeshDissolver; }
 
MStatus MeshDissolver::doIt(const MArgList& argList) {

    MStatus stat; 
    MSelectionList selection;
    char msg[100];

    MGlobal::getActiveSelectionList(selection);
    
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
        stat = collectFaceData(mdagPath, faceData);
        if (!checkStatus(stat)) return MS::kFailure;

        MFnMesh surfFn;
        surfFn.create(faceData->numVertices, faceData->numPolygons, faceData->vertexArray,
                      faceData->polygonCounts, faceData->polygonConnects, mTransform);
    }

    return MS::kSuccess;
}

/// Takes an MDagPath and a FaceData* as parameter. The function uses the mesh at the
/// MDagPath to populate the data in FaceData*.
/// The function handles meshes with varying number of vertices per face, e.g. spheres.
/// 
/// Returns MS::kSuccess if successfully executed. 
MStatus MeshDissolver::collectFaceData(const MDagPath& mdagPath, FaceData* faceData) {

    MStatus stat;
    MItMeshPolygon faceIter(mdagPath, MObject::kNullObj, &stat);
    if (!checkStatus(stat)) return stat;

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

    return MS::kSuccess;
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
    MGlobal::displayInfo("~MeshDissolver");
    delete faceData;
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