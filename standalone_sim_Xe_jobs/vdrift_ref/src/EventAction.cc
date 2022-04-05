// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#include "G4Event.hh"
#include "Randomize.hh"

#include "EventAction.hh"
#include "RunAction.hh"
#include "g4root.hh"

EventAction::EventAction(RunAction* run)
:fRun(run)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
  fRun->SetNumEvent(evtNb);
}

void EventAction::EndOfEventAction(const G4Event* )
{  

}
