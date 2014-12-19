#include "GenerateParticleCommand.h"
#include "ExtractFaceCommand.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "August Ek & Ramin Assadi", "1.0", "Any");

    MStatus status = plugin.registerCommand("extract", ExtractFaceCommand::creator); 
    CHECK_MSTATUS_AND_RETURN_IT(status);
    
    status = plugin.registerCommand("mesh2particle", GenerateParticleCommand::creator); 
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}
 
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);

    MStatus status = plugin.deregisterCommand("extract");
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.deregisterCommand("mesh2particle");
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}

int main(int argc, char** argv) {

    if (argc != 2) return 0;
    return 0;
}