#include "MeshDissolver.h"
#include <maya/MFnPlugin.h>

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

// Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn) 
// won't initalize plugin with implicit entry point.
#ifdef __APPLE__ 

int main(int argc, char** argv) {

    if (argc != 2) return 0;
    return 0;
}

#endif // __APPLE__