#include "GenerateParticleCommand.h"
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>
#include <maya/MFnTransform.h>
#include <sstream>

GenerateParticleCommand::GenerateParticleCommand() {

}

GenerateParticleCommand::~GenerateParticleCommand() {

}

MStatus GenerateParticleCommand::doIt(const MArgList& argList) {

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

    	// Collect vertex position data.
    	stat = collectMeshData(mdagPath, &pts);

    	if (checkStatus(stat)) {
    		// Look for flags.
    		int flagIndex = argList.flagIndex("n", "nParticle");

    		// If flag is found, create nParticles, otherwise create standard particles.
    		if (flagIndex != -1) {
    			createNClothParticles(pts);
    		} else {
    			stat = createParticles(pts, mdagPath);
    		}

    		// Delete the original mesh.
    		deleteMesh(mdagPath);
    	} 
	}
	return stat;
}


MStatus GenerateParticleCommand::redoIt() {
	return MS::kSuccess;
}

void* GenerateParticleCommand::creator() {
	return new GenerateParticleCommand;
}

MStatus GenerateParticleCommand::collectMeshData(const MDagPath& mdagPath, MPointArray* dest) {
	MStatus stat;
	MPoint pt;
	MItMeshVertex vertIter(mdagPath, MObject::kNullObj, &stat);
	if(!checkStatus(stat)) return MS::kFailure;

	 for (; !vertIter.isDone(); vertIter.next()) {
    	pt = vertIter.position(MSpace::kWorld);
    	dest->append(pt);
    }
    return MS::kSuccess;
}

std::string GenerateParticleCommand::buildVerticePositionString(const MPointArray& pts, MStatus* stat) {
    MPoint pt;
    int count = 0;
    std::stringstream ss;
    ss << "[";	
    for (int i = 0, len = pts.length(); i < len; i++) {
    	pt = pts[i];
    	ss << "(" << pt[0] << ", " << pt[1] << ", " << pt[2] << ")";
		if(count < len - 1) {
			ss << ", ";
		} 
    	count++;
    }
    ss << "]";
	return ss.str();
}

void GenerateParticleCommand::createNClothParticles(const MPointArray& pts) {
		std::stringstream ss;

		// Concatenate the string. 
		ss << "cmds.nParticle( p=" << buildVerticePositionString(pts) << ")";
		std::string pystring = ss.str();

		// Cast to MString and execute on idle.
		MString commandToExecute(pystring.c_str());
		MGlobal::executePythonCommandOnIdle(commandToExecute);	
}

MStatus GenerateParticleCommand::createParticles(const MPointArray& pts, MDagPath& mdagPath) {
		MStatus stat;
		MFnTransform transformFn(mdagPath.node());
    	MTransformationMatrix parentTransform = transformFn.transformation(&stat);

		if (checkStatus(stat)) {

			// Create particle prtSystemm
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
			return MS::kSuccess;
		} else {
			return MS::kFailure;
		}
}

MStatus GenerateParticleCommand::deleteMesh(MDagPath& object) {
	MStatus stat;
    MString deleteOriginalStr = "delete " + object.fullPathName();
    stat = MGlobal::executeCommand(deleteOriginalStr);
    if(!checkStatus(stat)) return MS::kFailure;
    return MS::kSuccess;
}

bool GenerateParticleCommand::checkStatus(const MStatus& stat) { 
    if (stat != MS::kSuccess) {
        MGlobal::displayError(stat.errorString());
        return false;
    }
    return true;
}