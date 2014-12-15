#include "ParticleConnector.h"
#include <maya/MTypes.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshVertex.h>

ParticleConnector::ParticleConnector() {

}

ParticleConnector::~ParticleConnector() {

}

MStatus ParticleConnector::doIt(const MArgList& argList) {

	MStatus stat;
	MPointArray pts;
	stat = collectMeshData(&pts);
	return MS::kSuccess;
}


MStatus ParticleConnector::redoIt() {
	return MS::kSuccess;
}

void* ParticleConnector::creator() {
	return new ParticleConnector;
}

bool ParticleConnector::checkStatus (const MStatus& stat) { 
    if (stat != MS::kSuccess) {
        MGlobal::displayError(stat.errorString());
        return false;
    }
    return true;
}

/// Collects the positions for the selected mesh and stores
/// it in the given MPointArray.
MStatus ParticleConnector::collectMeshData(MPointArray* dest) {
	MStatus stat;
	MDagPath mdagPath;
    MPoint pt;

	// Get mesh from selection
	MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);

    // Get DagPath of the first object.
    selection.getDagPath(0, mdagPath);

    // Create iterator on the current dagpath
    MItMeshVertex vertIter(mdagPath, MObject::kNullObj, &stat);
    if(!checkStatus(stat)) return MS::kFailure;

    // Iterate through vertices and append their positions to dest.
    for (; !vertIter.isDone(); vertIter.next()) {
    	pt = vertIter.position(MSpace::kWorld, &stat);
    	dest->append(pt);
    }
	return MS::kSuccess;
}