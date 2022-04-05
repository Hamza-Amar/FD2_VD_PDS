#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction, double x, double y, double z)
 : G4VUserActionInitialization(),
   fDetectorConstruction(detConstruction)
{x0=x; y0=y; z0=z;}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{

}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction(x0,y0,z0));
  
  RunAction* runAction= new RunAction(fDetectorConstruction);
  SetUserAction(runAction);

  SetUserAction(new EventAction(runAction));
  
  SetUserAction(new SteppingAction(runAction,fDetectorConstruction));
}  

