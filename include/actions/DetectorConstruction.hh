#pragma once

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override; // abstract method (from base class G4VUserDetectorConstruction)

    G4LogicalVolume* GetCaloLV() const { return fCaloLV; }
    G4LogicalVolume* GetSipmLV() const { return fSipmLV; }

  protected:
    G4LogicalVolume* fCaloLV = nullptr;
    G4LogicalVolume* fSipmLV = nullptr;
};