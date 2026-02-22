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
/// \file B1RunAction.hh
/// \brief Definition of the B1RunAction class

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
//#include "g4root.hh" - This file is for old versions.
#include "G4AnalysisManager.hh"

class G4Run;

/// Run action class
///
//// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class B1RunAction : public G4UserRunAction
{
  public:
    B1RunAction();
    virtual ~B1RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddEdep_50keV (G4double edep);
    void AddEdep_60keV (G4double edep);
    void AddEdep_75keV (G4double edep);
    void AddEdep_100keV (G4double edep);
    void AddEdep_DepE (G4double edep);
    void Add50keV () { f50keV++; };
    void Add60keV () { f60keV++; };
    void Add75keV () { f75keV++; };
    void Add100keV () { f100keV++; };


  private:
    G4Accumulable<G4double> fEdep_50keV;
    G4Accumulable<G4double> fEdep_60keV;
    G4Accumulable<G4double> fEdep_75keV;
    G4Accumulable<G4double> fEdep_100keV;
    G4Accumulable<G4double> fEdep_DepE;
    G4Accumulable<G4int> f50keV = 0.;
    G4Accumulable<G4int> f60keV = 0.;
    G4Accumulable<G4int> f75keV = 0.;
    G4Accumulable<G4int> f100keV = 0.;
    G4String fName;
};

#endif

