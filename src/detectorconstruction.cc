#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
    fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    // Helium vessel parameters
    //
    G4double ves_sizeX = 15 * m, ves_sizeY = 4 * m, ves_sizeZ = 11 * m;
    G4Material* ves_mat = nist->FindOrBuildMaterial("G4_Helium");

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //     
    // World
    //
    G4double world_sizeX = 1.2 * ves_sizeX;
    G4double world_sizeY = 1.2 * ves_sizeY;
    G4double world_sizeZ = 1.2 * ves_sizeZ;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld =
        new G4Box("World",                       //its name
            0.5 * world_sizeX, 0.5 * world_sizeY, 0.5 * world_sizeZ);     //its size

    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld,          //its solid
            world_mat,           //its material
            "World");            //its name

    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0,                     //no rotation
            G4ThreeVector(),       //at (0,0,0)
            logicWorld,            //its logical volume
            "World",               //its name
            0,                     //its mother  volume
            false,                 //no boolean operation
            0,                     //copy number
            checkOverlaps);        //overlaps checking

//     
// Vessel
//  
    G4Box* solidVes =
        new G4Box("Vessel",                    //its name
            0.5 * ves_sizeX, 0.5 * ves_sizeY, 0.5 * ves_sizeZ); //its size

    G4LogicalVolume* logicVes =
        new G4LogicalVolume(solidVes,            //its solid
            ves_mat,             //its material
            "Vessel");         //its name

    new G4PVPlacement(0,                       //no rotation
        G4ThreeVector(),         //at (0,0,0)
        logicVes,                //its logical volume
        "Vessel",              //its name
        logicWorld,              //its mother  volume
        false,                   //no boolean operation
        0,                       //copy number
        checkOverlaps);          //overlaps checking

//     
// Carbon target 
//  
    G4Material* target_mat = nist->FindOrBuildMaterial("G4_Carbon");
    G4ThreeVector pos1 = G4ThreeVector(0, 0, 0);

    // Conical section shape       
    G4double target_rmin = 0. * cm, target_rmax = 1.3 * cm;
 
    G4double target_hz = 45.7 * cm;
    G4double target_phimin = 0. * deg, target_phimax = 360. * deg;
    G4Cons* solidTarget =
        new G4Tubs("Target",
            target_rmin, target_rmax, target_hz,
            target_phimin, target_phimax);

    G4LogicalVolume* logicTarget =
        new G4LogicalVolume(solidTarget,         //its solid
            target_mat,          //its material
            "Target");           //its name

    new G4PVPlacement(0,                       //no rotation
        pos1,                    //at position
        logicTarget,             //its logical volume
        "Target",                //its name
        logicVes,                //its mother  volume
        false,                   //no boolean operation
        0,                       //copy number
        checkOverlaps);          //overlaps checking

//     


// Set Vessel as scoring volume
//
    fScoringVolume = logicVes;

    //
    //always return the physical World
    //
    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
