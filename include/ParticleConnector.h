#ifndef PARTICLE_CONNECTOR_H
#define PARTICLE_CONNECTOR_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>
#include <maya/MPointArray.h>

class ParticleConnector : public MPxCommand {
  public:
    ParticleConnector();
    ~ParticleConnector();
    virtual MStatus doIt(const MArgList& argList);
    virtual MStatus redoIt();
    static void* creator();
   
  private:
  	MStatus collectMeshData(MPointArray* dest);
  	bool checkStatus(const MStatus& stat);

};

#endif // PARTICLE_CONNECTOR_H