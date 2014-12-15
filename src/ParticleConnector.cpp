#include "ParticleConnector.h"
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MPlug.h>

ParticleConnector::ParticleConnector() {

}

ParticleConnector::~ParticleConnector() {

}

MStatus ParticleConnector::doIt(const MArgList& argList) {

	MStatus stat = MS::kFailure;
	MPointArray pts;
	stat = collectMeshData(&pts);
	if (checkStatus(stat)) {

		// Create particle system
		MFnParticleSystem prtSystem;

		// Set the was object the particleSystem should have
		MObject particle = prtSystem.create();
		prtSystem.setObject(particle);
		
		// Emit all particles
		prtSystem.emit(pts);

		// Change the particles render type
		MFnDependencyNode fromNode(particle);
		MPlug plug = fromNode.findPlug("particleRenderType");
		plug.setValue(SPHERES);

		// Save state so the particles do not disappear
		prtSystem.saveInitialState();
		stat = MS::kSuccess;
	}
	return stat;
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