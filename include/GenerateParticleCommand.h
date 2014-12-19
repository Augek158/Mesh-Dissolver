#ifndef GENERATE_PARTICLE_COMMAND_H
#define GENERATE_PARTICLE_COMMAND_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MPointArray.h>
#include <maya/MDagPath.h>

#include <string>

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

class GenerateParticleCommand : public MPxCommand {
  public:
    GenerateParticleCommand();
    ~GenerateParticleCommand();
    virtual MStatus doIt(const MArgList& argList);
    virtual MStatus redoIt();
    static void* creator();
   
  private:

  	/// Iterates through a mesh and saves all vertex positions to an MPointArray.
  	MStatus collectMeshData(const MDagPath& mdagPath, MPointArray* dest);

    /// Takes an array of points and converts it to a string that can be used as a 
    /// Python command.
  	std::string buildVerticePositionString(const MPointArray& pts, MStatus* stat = NULL);

  	/// Creates nCloth particles using a python call.
  	void createNClothParticles(const MPointArray& pts);

  	/// Creates standard Maya particles.
  	MStatus createParticles(const MPointArray& pts, MDagPath& mdagPath);

  	/// Deletes the mesh connected to the dagpath by using a MEL-command.
  	MStatus deleteMesh(MDagPath& object);

  	/// Utility function to check if a status is ok.
  	bool checkStatus(const MStatus& stat);

};

#endif // GENERATE_PARTICLE_COMMAND_H