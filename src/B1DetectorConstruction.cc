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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4Region.hh"
#include "G4ProductionCuts.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

G4Material* MakeIohexolPure()
{
  auto nist = G4NistManager::Instance();

  auto elC = nist->FindOrBuildElement("C");
  auto elH = nist->FindOrBuildElement("H");
  auto elI = nist->FindOrBuildElement("I");
  auto elN = nist->FindOrBuildElement("N");
  auto elO = nist->FindOrBuildElement("O");

  G4double density = 1.39 * g/cm3;  // TODO: 실험 기반 값으로 조정

  auto iohexol = new G4Material("Iohexol_100", density, 5);
  iohexol->AddElement(elC, 19);
  iohexol->AddElement(elH, 26);
  iohexol->AddElement(elI, 3);
  iohexol->AddElement(elN, 3);
  iohexol->AddElement(elO, 9);

  return iohexol;
}

//bckim contrast only

////////////////////////////////////////////////////////////////////////////////////////////////////////////

G4Material* MakeIohexol90Water10()
{
  auto nist = G4NistManager::Instance();

  auto water   = nist->FindOrBuildMaterial("G4_WATER");
  auto iohexol = MakeIohexolPure();

  G4double density = 1.35 * g/cm3;

  auto mix = new G4Material("Iohexol90_Water10", density, 2);
  mix->AddMaterial(iohexol, 0.90);
  mix->AddMaterial(water,   0.10);

  return mix;
}

// bckim contrast 90%+water 10%

////////////////////////////////////////////////////////////////////////////////////////////////////////

G4Material* MakeIohexol80Water20()
{
  auto nist = G4NistManager::Instance();

  auto water   = nist->FindOrBuildMaterial("G4_WATER");
  auto iohexol = MakeIohexolPure();

  G4double density = 1.31 * g/cm3;

  auto mix = new G4Material("Iohexol80_Water20", density, 2);
  mix->AddMaterial(iohexol, 0.80);
  mix->AddMaterial(water,   0.20);

  return mix;
}

//bckim contrast 80%+water 20%

//////////////////////////////////////////////////////////////////////////////////////////////////////

G4Material* MakeIohexol70Water30()
{
  auto nist = G4NistManager::Instance();

  auto water   = nist->FindOrBuildMaterial("G4_WATER");
  auto iohexol = MakeIohexolPure();

  G4double density = 1.27 * g/cm3;

  auto mix = new G4Material("Iohexol70_Water30", density, 2);
  mix->AddMaterial(iohexol, 0.70);
  mix->AddMaterial(water,   0.30);

  return mix;
}

//bckim contrast 70%+water 30%

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  G4double env_sizeY = 10*cm, env_sizeX = 10*cm, env_sizeZ = 10*cm;
	
  G4bool checkOverlaps = true;

  //     
  // World
  //G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeX  = 4.0*env_sizeX;
  G4double world_sizeY  = 4.0*env_sizeY;
  G4double world_sizeZ  = 12.0*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);     //its size
      // 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,0),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  // Acrylic Plate

  G4Material* acrylic = nist->FindOrBuildMaterial("G4_PLEXIGLASS");


  //     
  // Plate
  //
  //
  G4Box* solidPlate =    
    new G4Box("Plate",                       //its name
        0.36*0.5*env_sizeX, 0.56*0.5*env_sizeY, 0.03*0.5*env_sizeZ); //its size
  
  G4LogicalVolume* logicPlate =                         
    new G4LogicalVolume(solidPlate,          //its solid
                        acrylic,             //its material
                        "Plate");            //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,400),  //at (0,0,400) Unit: mm
                    logicPlate,              //its logical volume
                    "Plate",                 //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  
  // C4-LS  
  //
  //
  //
  //G4Material* LS = nist->FindOrBuildMaterial("G4_WATER");
  //
  //
  G4Material* LS = MakeIohexol70Water30();
  //
  G4Box* solidC4_LS =    
    new G4Box("C4_LS",                       //its name
        0.3*0.5*env_sizeX, 0.5*0.5*env_sizeY, 0.15*0.5*env_sizeZ); //its size
  
  G4LogicalVolume* logicC4_LS =                         
    new G4LogicalVolume(solidC4_LS,          //its solid
                        LS,                  //its material
                        "C4_LS");            //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,390),  //at (0,0,390) Unit: mm
                    logicC4_LS,              //its logical volume
                    "C4_LS",                 //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  
  // Detector
  //
  //
  //
  G4double innerRadius = 0. * cm;
  G4double outerRadius = 0.65 * cm;
  G4double hz = 0.2 * cm;
  G4double startAngle = 0. * cm;
  G4double spanningAngle = 360. * cm;
  
  G4Material* detector_mat = nist->FindOrBuildMaterial("G4_Pb");

  G4Tubs* solidDetector = 
    new G4Tubs("Detector",
	innerRadius,
	outerRadius,
	hz,
	startAngle,
	spanningAngle);

  G4LogicalVolume* logicDetector = 
    new G4LogicalVolume(solidDetector,
		        detector_mat,
		        "Detector");

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,409),  //at (0,0,409) Unit: mm
                    logicDetector,           //its logical volume
                    "Detector",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
		    



fScoringVolume = logicDetector;
//  fScoringVolume = logicShape2;
fRegion = new G4Region("Envelope");
G4ProductionCuts* cuts = new G4ProductionCuts();
G4double defCut = 1*nanometer;
cuts->SetProductionCut(defCut,"gamma");
cuts->SetProductionCut(defCut,"e-");
cuts->SetProductionCut(defCut,"e+");
cuts->SetProductionCut(defCut,"proton");
//fRegion->AddRootLogicalVolume(logicC4_LS); 
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo.:.......oooOO0OOooo........:oooOO0OOooo........oooOO0OOooo......
