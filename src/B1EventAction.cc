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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep_50keV(0.),
  fEdep_60keV(0.),
  fEdep_75keV(0.),
  fEdep_100keV(0.),
  fEdep_DepE(0.)
{
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event* event)
{    
  fEdep_50keV = 0.;
  fEdep_60keV = 0.;
  fEdep_75keV = 0.;
  fEdep_100keV = 0.;
  fEdep_DepE = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event* event)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep_50keV(fEdep_50keV);
  fRunAction->AddEdep_60keV(fEdep_60keV);
  fRunAction->AddEdep_75keV(fEdep_75keV);
  fRunAction->AddEdep_100keV(fEdep_100keV);
  fRunAction->AddEdep_DepE(fEdep_DepE);
  
  //G4double initialEnergy = 50.0 + (event->GetEventID()/1000*1.0);
  G4double initialEnergy = 30; 

  auto analysisManager = G4AnalysisManager::Instance();
//  analysisManager->FillNtupleDColumn(0, initialEnergy);
//  analysisManager->FillNtupleDColumn(1, fEdep_DepE);
  analysisManager->FillNtupleDColumn(0, fEdep_DepE); //You should make a different variable.
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
