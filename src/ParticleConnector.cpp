#include "ParticleConnector.h"
#include <maya/MGlobal.h>

ParticleConnector::ParticleConnector() {

}

ParticleConnector::~ParticleConnector() {

}

MStatus ParticleConnector::doIt(const MArgList& argList) {
	MGlobal::displayInfo("Hello ParticleConnector!");

	// Your fancy code here
	
	return MS::kSuccess;
}

MStatus ParticleConnector::redoIt() {
	return MS::kSuccess;
}

void* ParticleConnector::creator() {
	return new ParticleConnector;
}