#include "ParticleConnector.h"
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>

ParticleConnector::ParticleConnector() {

}

ParticleConnector::~ParticleConnector() {

}

MStatus ParticleConnector::doIt(const MArgList& argList) {

	MStatus stat = MS::kFailure;

	// Create iterator on selection. 
	MSelectionList selection;
    MGlobal::getActiveSelectionList(selection);
    MItSelectionList iter(selection);

    // Iterate through selected meshes and convert to particles.
    for (; !iter.isDone(); iter.next()) {
    	MDagPath mdagPath;
    	MPointArray pts;
    	iter.getDagPath(mdagPath);
    	stat = collectMeshData(mdagPath, &pts);

		if (checkStatus(stat)) {

			// Create particle system
			MFnParticleSystem prtSystem;

			// Create particle system with the same parent as the source mesh.
			MObject particle = prtSystem.create(mdagPath.node());
			prtSystem.setObject(particle);
			
			// Emit all particles
			prtSystem.emit(pts);

			// Change the particles render type
			MFnDependencyNode fromNode(particle);
			MPlug plug = fromNode.findPlug("particleRenderType");
			plug.setValue(SPHERES);

			// Save state so the particles do not disappear
			prtSystem.saveInitialState();
			stat = deleteMesh(mdagPath);
			if(!checkStatus(stat)) break;
		}		
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

/// Deletes the mesh connected to the dagpath by
/// using a MEL-command to delete it.
MStatus ParticleConnector::deleteMesh(MDagPath& object) {
	MStatus stat;
	object.extendToShapeDirectlyBelow(0);
    MString deleteOriginalStr = "delete " + object.fullPathName();
    // MGlobal::displayInfo(object.node().apiTypeStr());
    stat = MGlobal::executeCommand(deleteOriginalStr);
    if(!checkStatus(stat)) return MS::kFailure;
    return MS::kSuccess;
}



/// Collects the positions for the selected mesh and stores
/// it in the given MPointArray.
MStatus ParticleConnector::collectMeshData(const MDagPath& mdagPath, MPointArray* dest) {
	MStatus stat;
    MPoint pt;

    // Create iterator on the current dagpath.
    MItMeshVertex vertIter(mdagPath, MObject::kNullObj, &stat);
    if(!checkStatus(stat)) return MS::kFailure;

    // Iterate through vertices and append their positions to dest.
    for (; !vertIter.isDone(); vertIter.next()) {
    	pt = vertIter.position(MSpace::kWorld, &stat);
    	dest->append(pt);
    }
	return MS::kSuccess;
}