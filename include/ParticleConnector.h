#ifndef PARTICLE_CONNECTOR_H
#define PARTICLE_CONNECTOR_H
 
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>

class ParticleConnector : public MPxCommand {
  public:
    ParticleConnector();
    ~ParticleConnector();
    virtual MStatus doIt(const MArgList& argList);
    virtual MStatus redoIt();
    static void* creator();
   
  private:

};

#endif // PARTICLE_CONNECTOR_H