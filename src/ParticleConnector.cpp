#include "ParticleConnector.h"
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MPlug.h>

ParticleConnector::ParticleConnector() {

}

ParticleConnector::~ParticleConnector() {

}

enum {
	MULTIPOINT,
	MULTISTREAK,
	NUMERICS,
	POINTSS,
	SPHERES,
	SPRITES,
	STREAKS,
	BLOBBYSURFACES,
	CLOUD,
	TUBE
};

MStatus ParticleConnector::doIt(const MArgList& argList) {
	MPointArray pointArray;
	MVector vec1 = MVector(1.0, 1.0, 1.0);
	MVector vec2 = MVector(1.0, 2.0, 3.0);
	MPoint point1(vec1);
	MPoint point2(vec2);

	pointArray.append(point1);
	pointArray.append(point2);
	
	// Create particle system
	MFnParticleSystem prtSystem;

	// Set the was object the particleSystem should have
	MObject particle = prtSystem.create();
	prtSystem.setObject(particle);
	
	// Emit all particles
	prtSystem.emit(pointArray);

	// Change the particles render type
	MFnDependencyNode fromNode(particle);
	MPlug plug = fromNode.findPlug("particleRenderType");
	plug.setValue(SPHERES);

	// Save state so the particles do not disappear
	prtSystem.saveInitialState();

	return MS::kSuccess;
}

MStatus ParticleConnector::redoIt() {
	return MS::kSuccess;
}

void* ParticleConnector::creator() {
	return new ParticleConnector;
}