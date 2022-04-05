// To run g4workshop
// make g4workshop_build
// cd g4workshop_build
// cmake -DGeant4_DIR=$G4COMP ../g4workshop_example
// make -j
// ./g4workshop

#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
  #include "G4VisExecutive.hh"
#endif

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include <stdlib.h>

int main(int argc,char** argv) {

  // Choose the Random engine
  //  
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  //set random seed with system time
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);  
  // Construct the default run manager

#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
   runManager->SetNumberOfThreads(1);
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory user initialization classes
  
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  
  PhysicsList* physics = new PhysicsList();

  runManager->SetUserInitialization(physics);    
  // User action initialization

  double x = atof(argv[1]);
  double y = atof(argv[2]);
  double z = atof(argv[3]);
  runManager->SetUserInitialization(new ActionInitialization(detector,x,y,z));
  
  // Initialize G4 kernel
  
  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager 
  
  G4UImanager* UImanager = G4UImanager::GetUIpointer(); 
  
  if (argc==1+3)   // Define UI session for interactive mode.
  { 
#ifdef _WIN32
    G4UIsession * session = new G4UIterminal();
#else
    G4UIsession * session = new G4UIterminal(new G4UItcsh);
#endif
    UImanager->ApplyCommand("/control/execute vdrift_build/g4workshop.mac");    
    //session->SessionStart();
    //delete session;
  }
  else           // Batch mode
  { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
  
}

