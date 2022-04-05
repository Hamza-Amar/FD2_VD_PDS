#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"

class PhysicsListMessenger;

class PhysicsList : public G4VUserPhysicsList
{
  public:
    PhysicsList();
    virtual ~PhysicsList();

  public:
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    virtual void SetCuts();

    void ConstructDecay();
    void ConstructEM();
    void ConstructOp();

    void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);
 
  private:
    G4int                fVerboseLebel;
    PhysicsListMessenger* fMessenger;
    G4int fMaxNumPhotonStep;
};

#endif /* PhysicsList_h */
