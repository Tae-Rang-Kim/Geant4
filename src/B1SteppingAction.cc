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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4EventManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction, B1RunAction* runAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fRunAction(runAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
    
    const B1PrimaryGeneratorAction* generatorAction
	    = static_cast<const B1PrimaryGeneratorAction*>
	    (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

    G4double gunEnergy = generatorAction->GetParticleGun()->GetParticleEnergy();

    if (!fScoringVolume) {
    fScoringVolume
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction())
	->GetScoringVolume();
    }

     // get volume of the current step
    G4LogicalVolume* volume
      = step->GetPreStepPoint()->GetTouchableHandle()
        ->GetVolume()->GetLogicalVolume();


    G4StepPoint* postPoint = step->GetPostStepPoint();
    G4VPhysicalVolume* postPV = postPoint->GetPhysicalVolume();
    
    if (!postPV) return;
/*
    G4Track* track = step->GetTrack();

    G4double initialEnergy = track->GetVertexKineticEnergy();
*/

// Energy deposited at C4-LS
    if(postPV && postPV->GetName() == "C4_LS")     fEventAction->AddEdep_DepE(step->GetTotalEnergyDeposit());
    



// 50keV
    if (std::abs(gunEnergy - 30. * keV) < 0.1 * keV){    
       if(postPoint->GetStepStatus() == fGeomBoundary && postPV && postPV->GetName() == "Detector")
    {
       fRunAction->Add50keV(); // Count up for Entrance of Priamary Particles 
       }

       if(postPV->GetName() == "Detector")       fEventAction->AddEdep_50keV(step->GetTotalEnergyDeposit());
    }

// 60keV
    if (std::abs(gunEnergy - 35. * keV) < 0.1 * keV){    
       if(postPoint->GetStepStatus() == fGeomBoundary && postPV && postPV->GetName() == "Detector")
    {
       fRunAction->Add60keV(); // Count up for Entrance of Priamary Particles 
       }

       if(postPV->GetName() == "Detector"){
       fEventAction->AddEdep_60keV(step->GetTotalEnergyDeposit());
       }
    }

// 75keV
    if (std::abs(gunEnergy - 45. * keV) < 0.1 * keV){    
       if(postPoint->GetStepStatus() == fGeomBoundary && postPV && postPV->GetName() == "Detector")
    {
       fRunAction->Add75keV(); // Count up for Entrance of Priamary Particles 
       }

       if(postPV->GetName() == "Detector"){
       fEventAction->AddEdep_75keV(step->GetTotalEnergyDeposit());
       }
    }

// 100keV
    if (std::abs(gunEnergy - 75. * keV) < 0.1 * keV){    
       if(postPoint->GetStepStatus() == fGeomBoundary && postPV && postPV->GetName() == "Detector")
    {
       fRunAction->Add100keV(); // Count up for Entrance of Priamary Particles 
       }

       if(postPV->GetName() == "Detector"){
       fEventAction->AddEdep_100keV(step->GetTotalEnergyDeposit());
       }
    }

 /*
  if (Track ID == 1 && Parent ID ==0 && kin_E == 0); {z_end = z}
  { 
	  analysisManager->FillNtupleDColumn(7, z_end);
  }
  */
		  //////
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  
