#ifndef PARTICLE_CONNECTOR_H
#define PARTICLE_CONNECTOR_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MPointArray.h>
#include <maya/MDagPath.h>

enum ParticleTypes {
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

class ParticleConnector : public MPxCommand {
  public:
    ParticleConnector();
    ~ParticleConnector();
    virtual MStatus doIt(const MArgList& argList);
    virtual MStatus redoIt();
    static void* creator();
   
  private:
  	MStatus collectMeshData(const MDagPath& mdagPath, MPointArray* dest);
  	MStatus deleteMesh(MDagPath& object);
  	bool checkStatus(const MStatus& stat);

};

#endif // PARTICLE_CONNECTOR_H