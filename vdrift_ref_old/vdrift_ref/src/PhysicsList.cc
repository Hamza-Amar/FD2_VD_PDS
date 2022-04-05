#include "globals.hh"
#include "PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ProcessManager.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
 
PhysicsList::PhysicsList() 
 : G4VUserPhysicsList(),
   fVerboseLebel(1), fMaxNumPhotonStep(20)
{
}

PhysicsList::~PhysicsList() { }

void PhysicsList::ConstructParticle()
{

  G4BosonConstructor bConstructor;
  bConstructor.ConstructParticle();

  G4LeptonConstructor lConstructor;
  lConstructor.ConstructParticle();

  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();

  G4BaryonConstructor rConstructor;
  rConstructor.ConstructParticle();

  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle(); 
}

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructDecay();
  ConstructEM();
  ConstructOp();
}

#include "G4Decay.hh"

void PhysicsList::ConstructDecay()
{
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

void PhysicsList::ConstructEM()
{
  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
    // gamma
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
    //electron
      // Construct processes for electron
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);

    } else if (particleName == "e+") {
    //positron
      // Construct processes for positron
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1, 4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon
     // Construct processes for muon
     pmanager->AddProcess(new G4MuMultipleScattering(),-1, 1, 1);
     pmanager->AddProcess(new G4MuIonisation(),      -1, 2, 2);
     pmanager->AddProcess(new G4MuBremsstrahlung(),  -1, 3, 3);
     pmanager->AddProcess(new G4MuPairProduction(),  -1, 4, 4);

    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino") &&
          !particle->IsShortLived()) {
       // all others charged particles except geantino
       pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
       pmanager->AddProcess(new G4hIonisation(),       -1,2,2);
     }
    }
  }
}

#include "G4Threading.hh"

void PhysicsList::ConstructOp()
{
  G4OpWLS* wlsProcess = new G4OpWLS();
  G4Cerenkov* cerenkovProcess = new G4Cerenkov("Cerenkov");
  cerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
  cerenkovProcess->SetMaxBetaChangePerStep(10.0);
  cerenkovProcess->SetTrackSecondariesFirst(true);
  G4Scintillation* scintillationProcess = new G4Scintillation("Scintillation");
  scintillationProcess->SetScintillationYieldFactor(1.);
  scintillationProcess->SetTrackSecondariesFirst(true);
  G4OpAbsorption* absorptionProcess = new G4OpAbsorption();
  G4OpRayleigh* rayleighScatteringProcess = new G4OpRayleigh();
  G4OpMieHG* mieHGScatteringProcess = new G4OpMieHG();
  G4OpBoundaryProcess* boundaryProcess = new G4OpBoundaryProcess();
  
  if(!G4Threading::IsWorkerThread())
  {
    G4EmSaturation* emSaturation =
              G4LossTableManager::Instance()->EmSaturation();
      scintillationProcess->AddSaturation(emSaturation);
  }

  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (cerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(cerenkovProcess);
      pmanager->SetProcessOrdering(cerenkovProcess,idxPostStep);
    }
    if (scintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(scintillationProcess);
      pmanager->SetProcessOrderingToLast(scintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(scintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(absorptionProcess);
      pmanager->AddDiscreteProcess(rayleighScatteringProcess);
      pmanager->AddDiscreteProcess(mieHGScatteringProcess);
      pmanager->AddDiscreteProcess(boundaryProcess);
      pmanager->AddDiscreteProcess(wlsProcess);
    }
  }
}

void PhysicsList::SetVerbose(G4int verbose)
{
  fVerboseLebel = verbose;
}

void PhysicsList::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
    fMaxNumPhotonStep = MaxNumber;
}

void PhysicsList::SetCuts()
{
  SetCutsWithDefault();

  if (verboseLevel>0) DumpCutValuesTable();
}

