#include "TROOT.h"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

using namespace Test;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  G4cout << "=================TEST SIMULATION=================" << G4endl;

  ROOT::EnableThreadSafety();

  // TODO: run header here


  // Seed generation for random
  std::chrono::high_resolution_clock clock;
  G4Random::setTheSeed(std::chrono::duration_cast<std::chrono::nanoseconds>(clock.now().time_since_epoch()).count());

  // RUN MANAGER
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default); // multithreaded run manager
  runManager->SetVerboseLevel(0);

  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // TODO: make physics list class, use monte carlo example and example b3a  TODO TODO TODO
  // Physics list
  G4VModularPhysicsList* physicsList = new FTFP_BERT(0); // 0 is for verbose level
  physicsList->RegisterPhysics(new G4OpticalPhysics()); // for optical photons
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());


  // INITIALIZATION
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->SetVerboseLevel(0);
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if (argc > 1) // batch mode
  {
    try // new: init.mac with beamOn shortcut
    {
      int nOfEvents = std::stoi(argv[1]);
      UImanager->ApplyCommand("/control/execute ../macros/init.mac");
      UImanager->ApplyCommand("/run/beamOn " + std::to_string(nOfEvents));
    }
    catch (...)
    {
      G4String fileName = argv[1]; // execute: ./test ../macros/###.mac
      UImanager->ApplyCommand("/control/execute " + fileName);
    }
  }
  else // interactive mode
  {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute ../macros/vis.mac");
    ui->SessionStart();
    delete ui;
  }


  // TERMINATION
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !
  delete visManager;
  delete runManager;

  return 0;
}