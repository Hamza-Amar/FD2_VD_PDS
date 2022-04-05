// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#include "PhysListEmLivermore.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// gamma

#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

// e-

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

// e+

#include "G4eplusAnnihilation.hh"

// mu

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

// hadrons, ions

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"

#include "G4SystemOfUnits.hh"

PhysListEmLivermore::PhysListEmLivermore(const G4String& name)
  :  G4VPhysicsConstructor(name)
{ }

PhysListEmLivermore::~PhysListEmLivermore()
{ }

void PhysListEmLivermore::ConstructProcess()
{
  // Add standard EM Processes
  auto aParticleIterator=GetParticleIterator();
  aParticleIterator->reset();
  while( (*aParticleIterator)() ){
    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    //Applicability range for Livermore models
    //for higher energies, the Standard models are used   
    G4double highEnergyLimit = 1*GeV;
         
    if (particleName == "gamma") {
      // gamma         

      G4PhotoElectricEffect* phot = new G4PhotoElectricEffect();
      G4LivermorePhotoElectricModel* 
      photModel = new G4LivermorePhotoElectricModel();
      photModel->SetHighEnergyLimit(highEnergyLimit);
      phot->AddEmModel(0, photModel);
      pmanager->AddDiscreteProcess(phot);

      G4ComptonScattering* compt = new G4ComptonScattering();
      G4LivermoreComptonModel* 
      comptModel = new G4LivermoreComptonModel();
      comptModel->SetHighEnergyLimit(highEnergyLimit);
      compt->AddEmModel(0, comptModel);
      pmanager->AddDiscreteProcess(compt);

      G4GammaConversion* conv = new G4GammaConversion();
      G4LivermoreGammaConversionModel* 
      convModel = new G4LivermoreGammaConversionModel();
      convModel->SetHighEnergyLimit(highEnergyLimit);
      conv->AddEmModel(0, convModel);
      pmanager->AddDiscreteProcess(conv);

      G4RayleighScattering* rayl = new G4RayleighScattering();
      G4LivermoreRayleighModel* 
      raylModel = new G4LivermoreRayleighModel();
      raylModel->SetHighEnergyLimit(highEnergyLimit);
      rayl->AddEmModel(0, raylModel);
      pmanager->AddDiscreteProcess(rayl);
      
    } else if (particleName == "e-") {
      //electron

      G4eIonisation* eIoni = new G4eIonisation();
      G4LivermoreIonisationModel* 
      eIoniModel = new G4LivermoreIonisationModel();
      eIoniModel->SetHighEnergyLimit(highEnergyLimit); 
      eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation() );
      pmanager->AddProcess(eIoni,                   -1, 1, 1);
      
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4LivermoreBremsstrahlungModel* 
      eBremModel = new G4LivermoreBremsstrahlungModel();
      eBremModel->SetHighEnergyLimit(highEnergyLimit);
      eBrem->AddEmModel(0, eBremModel);
      pmanager->AddProcess(eBrem,                   -1, 2, 2);
                  
    } else if (particleName == "e+") {
      //positron
      pmanager->AddProcess(new G4eIonisation,       -1, 1, 1);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 2, 2);
      pmanager->AddProcess(new G4eplusAnnihilation,  0,-1, 3);
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
      pmanager->AddProcess(new G4MuIonisation,      -1, 1, 1);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 2, 2);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 3, 3);       
     
    } else if( particleName == "alpha" || particleName == "GenericIon" ) { 
      pmanager->AddProcess(new G4ionIonisation,     -1, 1, 1);

    } else if ((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) && 
               (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      pmanager->AddProcess(new G4hIonisation,       -1, 1, 1);
    }
  }
}

