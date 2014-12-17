#include "ParticleConnector.h"
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>
#include <maya/MFnTransform.h>

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
    	MDagPath mdagPath, particleDag;
    	MPointArray pts;
    	iter.getDagPath(mdagPath);
    	stat = collectMeshData(mdagPath, &pts);

    	// Get the transformation matrix of the parent.
    	MFnTransform transformFn(mdagPath.node());
    	MTransformationMatrix parentTransform = transformFn.transformation(&stat);

		if (checkStatus(stat)) {

			// Create particle system
			MFnParticleSystem prtSystem(mdagPath);
			MObject particle = prtSystem.create();
			prtSystem.setObject(particle);

			// Create function set to manipulate dagNode of the particlesystem.
			MFnDagNode fnDagNode(particle);

			if (fnDagNode.parentCount() > 0) {
				// Sets the transformation to the one of its parent.
				// We can't explicitly set it in the MFnParticleSystem.create() method since
				// some attributes essential for the particle system are lost. 
				MObject parent = fnDagNode.parent(0);
				if (parent.apiType() == MFn::kTransform) {
					MFnTransform parentFn(parent);
					parentFn.set(parentTransform);
				}
			}

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

/// Utility function to check if a status is ok.
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
    MString deleteOriginalStr = "delete " + object.fullPathName();
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