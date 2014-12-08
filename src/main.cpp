#include "MeshDissolver.h"
#include "ParticleConnector.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "August Ek & Ramin Assadi", "0.2", "Any");

    MStatus status = plugin.registerCommand("dissolve", MeshDissolver::creator); 
    CHECK_MSTATUS_AND_RETURN_IT(status);
    
    status = plugin.registerCommand("connect", ParticleConnector::creator); 
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}
 
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);

    MStatus status = plugin.deregisterCommand("dissolve");
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.deregisterCommand("connect");
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}

int main(int argc, char** argv) {

    if (argc != 2) return 0;
    return 0;
}