//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"

//#include "g4root.hh" - This file is for old versions.
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction(),
  fEdep_50keV(0.),
  fEdep_60keV(0.),
  fEdep_75keV(0.),
  fEdep_100keV(0.),
  fEdep_DepE(0.),
  f50keV(0.), 
  f60keV(0.),
  f75keV(0.),
  f100keV(0.)	
	
{ 
  // add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep_50keV);
  accumulableManager->RegisterAccumulable(fEdep_60keV);
  accumulableManager->RegisterAccumulable(fEdep_75keV);
  accumulableManager->RegisterAccumulable(fEdep_100keV);
  accumulableManager->RegisterAccumulable(fEdep_DepE);
  
  // implementation for B.C. KIM request to "open file and create ntuple"
  

  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{
  // implementation for B.C. KIM request to "write_ntuple_and_close_file"
 

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  /////////////////////////////////////////////////////////////////
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager -> OpenFile("/home/kim_tae_rang/ResultOfB1.root");
  analysisManager -> CreateNtuple("Result1", "hist");
  analysisManager -> CreateNtupleFColumn("Track_Deposit_E");
  analysisManager -> CreateNtupleFColumn("Track_Lengnth");
  analysisManager -> CreateNtupleFColumn("Track_Kinetic_E");
  analysisManager -> CreateNtupleFColumn("x");
  analysisManager -> CreateNtupleFColumn("y");
  analysisManager -> CreateNtupleFColumn("z");
  analysisManager -> CreateNtupleIColumn("Track_ID");
  analysisManager -> CreateNtupleIColumn("Event_ID"); // T.R.Kim added. 
  analysisManager -> FinishNtuple();

  analysisManager -> CreateNtuple("Result2", "hitcounts");
  analysisManager -> CreateNtupleIColumn("Entrance Counts of Primary particles");
  analysisManager -> CreateNtupleIColumn("Entrance Counts of All particles");
  analysisManager -> CreateNtupleIColumn("Exit Counts of All particles");
  analysisManager -> CreateNtupleFColumn("Energy deposited in the envelope");
  analysisManager -> FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep_50keV  = fEdep_50keV.GetValue();
  G4double edep_60keV = fEdep_60keV.GetValue();
  G4double edep_75keV = fEdep_75keV.GetValue();
  G4double edep_100keV = fEdep_100keV.GetValue();
  G4double edep_DepE = fEdep_DepE.GetValue();
 /* 
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  

  const B1DetectorConstruction* detectorConstruction
   = static_cast<const B1DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;
*/
  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " Count of Hits with 50keV gamma: " << f50keV.GetValue() 
     << G4endl
     << " Count of Hits with 60keV gamma : " << f60keV.GetValue() 
     << G4endl
     << " Count of Hits with 75keV gamma: " << f75keV.GetValue() 
     << G4endl
     << " Count of Hits with 100keV gamma: " << f100keV.GetValue() 
     << G4endl
     << "Total Energy Deposit in Detector with 50keV: " << G4BestUnit( fEdep_50keV.GetValue(), "Energy" )
     << G4endl
     << "Total Energy Deposit in Detector with 60keV: " << G4BestUnit( fEdep_60keV.GetValue(), "Energy" )
     << G4endl
     << "Total Energy Deposit in Detector with 75keV: " << G4BestUnit( fEdep_75keV.GetValue(), "Energy" )
     << G4endl
     << "Total Energy Deposit in Detector with 100keV: " << G4BestUnit( fEdep_100keV.GetValue(), "Energy" )
     << G4endl
     << "Total Energy Deposit in C4_LS: " << G4BestUnit( fEdep_DepE.GetValue(), "Energy" )
     << G4endl
     << "------------------------------------------------------------"
     << G4endl;
  
  /////////////////////////////////////////////////////////////////
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager -> Write();  
  analysisManager -> CloseFile();

  //delete G4AnalysisManager::Instance(); G4AnalysisManager is sigleton object.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::AddEdep_50keV(G4double edep)
{
  fEdep_50keV  += edep;

}

void B1RunAction::AddEdep_60keV(G4double edep)
{
  fEdep_60keV  += edep;

}

void B1RunAction::AddEdep_75keV(G4double edep)
{
  fEdep_75keV  += edep;

}

void B1RunAction::AddEdep_100keV(G4double edep)
{
  fEdep_100keV  += edep;

}

void B1RunAction::AddEdep_DepE(G4double edep)
{
  fEdep_DepE += edep;

}

