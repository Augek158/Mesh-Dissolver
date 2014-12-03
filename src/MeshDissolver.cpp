#include "MeshDissolver.h"

// Maya includes
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MDagModifier.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>

// Maya function Sets
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>

// std
#include <vector>
 
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
        checkStatus(stat);

        // -------- Extract faces ----------//

        MPointArray ptArray;
        MFloatPointArray fPtArray;

        // TODO: Should be able to handle [3][4] as well.
        double dblPts[4][4];

        // Create iterator
        MItMeshPolygon faceIter(mdagPath);

        // TODO: Should be able to handle [3][4] as well.
        int arr[] = {0, 1, 2, 3};
        MIntArray indices = MIntArray(arr, 4);

        // Vector allocates on heap by default.
        std::vector<Face> vec;

        Face face;
        for(; !faceIter.isDone(); faceIter.next()) {
            face.numVertices = faceIter.polygonVertexCount();
            face.numPolygons = 1;

            // Get pointArray and cast it to MFloatPointArray.
            faceIter.getPoints(ptArray);
            ptArray.get(dblPts);
            face.vertexArray = MFloatPointArray(dblPts, face.numVertices);

            // Indices;
            face.polygonConnects = indices;

            // TODO: Should be able to handle [3][4] as well.
            face.polygonCounts = MIntArray(1, 4);

            // Texture coordinates
            faceIter.getUVs(face.uArray, face.vArray);

            vec.push_back(face);
        }

        MFnMesh surfFn;
        
        for (std::vector<Face>::iterator it = vec.begin(); it != vec.end(); ++it) {
            mObject = surfFn.create(it->numVertices, it->numPolygons, it->vertexArray, it->polygonCounts,
                                    it->polygonConnects, it->uArray, it->vArray, mTransform);
        }

        // ------- Extract Faces End -------- //
    }

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