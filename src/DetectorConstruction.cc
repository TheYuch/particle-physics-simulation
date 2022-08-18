#include "DetectorConstruction.hh"

#include <G4VisAttributes.hh>
#include <G4Colour.hh>

#include <G4RunManager.hh>
#include <G4NistManager.hh>

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>

#include <G4SDManager.hh>
#include "detectors/SensitiveDetector.hh"
#include "hitdata/CaloHitData.hh"
#include "hitdata/SipmHitData.hh"

namespace Test
{

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{
  delete fCaloLV;
  delete fSipmLV;
}

G4Material* getLYSO(std::vector<G4double>* pPhotonEnergy)
{
  G4int nComponents = 5;
  G4double density = 7.4*g/(cm*cm*cm);

  // fraction of elements by atomic number
  G4double x = 0.05; // ranges from 0 - 0.2 in paper
  G4double pCe = 0.01;
  G4double pO = (1 - pCe) * 5/8.;
  G4double pSi = (1 - pCe) * 1/8.;
  G4double pLu = (1 - pCe) * 2/8. * (1 - x);
  G4double pY = (1 - pCe) * 2/8. * (x);

  // mass of elements
  G4double wCe = 140.12;
  G4double wO = 15.999;
  G4double wSi = 28.085;
  G4double wLu = 174.97;
  G4double wY = 88.906;
  G4double totalWeight = (pCe * wCe) + (pO * wO) + (pSi * wSi) + (pLu * wLu) + (pY * wY);

  // fractional masses
  G4double fCe = (pCe * wCe) / (totalWeight);
  G4double fO = (pO * wO) / (totalWeight);
  G4double fSi = (pSi * wSi) / (totalWeight);
  G4double fLu = (pLu * wLu) / (totalWeight);
  G4double fY = (pY * wY) / (totalWeight);

  // create mixture
  G4Material* LYSO = new G4Material("LYSO", density, nComponents);
  G4NistManager* nist = G4NistManager::Instance();
  LYSO->AddElement(nist->FindOrBuildElement("Ce"), fCe);
  LYSO->AddElement(nist->FindOrBuildElement("O"), fO);
  LYSO->AddElement(nist->FindOrBuildElement("Si"), fSi);
  LYSO->AddElement(nist->FindOrBuildElement("Lu"), fLu);
  LYSO->AddElement(nist->FindOrBuildElement("Y"), fY);

  // define material properties
  std::vector<G4double> lysoRefractiveIndex = { 1.6, 1.7, 1.5 };
  std::vector<G4double> lysoAbsorptionLength = { 1*m, 1*m, 1*m };
  std::vector<G4double> lysoScintillation = { 1., 1., 1. };

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", *pPhotonEnergy, lysoRefractiveIndex, false, true);
  mpt->AddProperty("ABSLENGTH", *pPhotonEnergy, lysoAbsorptionLength, false, true);
  mpt->AddProperty("SCINTILLATIONCOMPONENT1", *pPhotonEnergy, lysoScintillation, false, true);

  mpt->AddConstProperty("SCINTILLATIONYIELD", 0. / MeV);
  mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1. * ns);
  mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  

  LYSO->SetMaterialPropertiesTable(mpt);


  return LYSO;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // General variables
  G4NistManager* nist = G4NistManager::Instance();

  G4bool checkOverlaps = false;

  //
  // Materials
  //
  G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
  std::vector<G4double> photonEnergy = { 2*eV, 3*eV, 4*eV };

  // LYSO
  G4Material* LYSO = getLYSO(&photonEnergy);

  // SiPM
  G4Material* sipmMat = nist->FindOrBuildMaterial("G4_Si");

  std::vector<G4double> sipmRefractiveIndex = { 3.3, 3.5, 3.9 };
  std::vector<G4double> sipmAbsorptionLength = { 2*nm, 0.5*nm, 1*nm };

  G4MaterialPropertiesTable* sipmMpt = new G4MaterialPropertiesTable();
  sipmMpt->AddProperty("RINDEX", photonEnergy, sipmRefractiveIndex, false, true);
  sipmMpt->AddProperty("ABSLENGTH", photonEnergy, sipmAbsorptionLength, false, true);

  sipmMat->SetMaterialPropertiesTable(sipmMpt);


  //
  // World
  //
  G4double hWorldLength = 30*cm;

  G4Box* worldS =
    new G4Box("World",
              hWorldLength, hWorldLength, hWorldLength);

  G4LogicalVolume* worldLV =
    new G4LogicalVolume(worldS,
                        air,
                        "World");

  G4VPhysicalVolume* worldPV =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      worldLV,              //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


  //
  // Calorimeter
  //

  // Physical Dimensions
  G4double hCaloXY = 5*cm;
  G4double hCaloLength = 15*cm;
  G4double hSipmXY = 5*cm;
  G4double hSipmLength = 2*cm;

  // Calorimeter-Envelope
  G4ThreeVector caloEnvPos = G4ThreeVector();

  G4Box* caloEnvS =
    new G4Box("Calorimeter-Envelope",
              hCaloXY, hCaloXY, hCaloLength + (2. * hSipmLength));
  
  G4LogicalVolume* caloEnvLV =
    new G4LogicalVolume(caloEnvS,
                        air,
                        "Calorimeter-Envelope");
  
  caloEnvLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.2)));
  
  new G4PVPlacement(0,
                    caloEnvPos,
                    caloEnvLV,
                    "Calorimeter-Envelope",
                    worldLV,
                    false,
                    0,
                    checkOverlaps);

  // Calorimeter
  G4ThreeVector caloPos = G4ThreeVector();

  G4Box* caloS =
    new G4Box("Calorimeter",
              hCaloXY, hCaloXY, hCaloLength);

  G4LogicalVolume* caloLV =
    new G4LogicalVolume(caloS,
                        LYSO,
                        "Calorimeter");
  
  caloLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 0.5, 0.0, 0.3)));

  new G4PVPlacement(0,
                    caloPos,
                    caloLV,
                    "Calorimeter",
                    caloEnvLV,
                    false,
                    0,
                    checkOverlaps);

  // SiPM
  G4ThreeVector sipmPos = G4ThreeVector(0, 0, hCaloLength + hSipmLength);

  G4Box* sipmS =
    new G4Box("SiPM",
              hSipmXY, hSipmXY, hSipmLength);
  
  G4LogicalVolume* sipmLV =
    new G4LogicalVolume(sipmS,
                        sipmMat,
                        "SiPM");
  
  sipmLV->SetVisAttributes(new G4VisAttributes(G4Colour(0.3, 0.6, 1.0, 0.6)));

  new G4PVPlacement(0,
                    sipmPos,
                    sipmLV,
                    "SiPM",
                    caloEnvLV,
                    false,
                    0,
                    checkOverlaps);

  //
  // Identifying special volumes
  //
  fCaloLV = caloLV;
  fSipmLV = sipmLV;

  // return world physical volume
  return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
  // TODO: use GDML file and map strings to HitData derived class types, then run a for loop to create all SDs

  SensitiveDetector<CaloHitData>* caloSD = new SensitiveDetector<CaloHitData>("Calorimeter");
  G4SDManager::GetSDMpointer()->AddNewDetector(caloSD);
  fCaloLV->SetSensitiveDetector(caloSD);

  SensitiveDetector<SipmHitData>* sipmSD = new SensitiveDetector<SipmHitData>("SiPM");
  G4SDManager::GetSDMpointer()->AddNewDetector(sipmSD);
  fSipmLV->SetSensitiveDetector(sipmSD);
}

}